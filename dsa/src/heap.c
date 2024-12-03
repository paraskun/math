#include <heap/heap.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define HEAP_L(i) (i * 2)
#define HEAP_R(i) (i * 2 + 1)
#define HEAP_P(i) (i / 2)

struct heap* heap_new(int cap, heap_fun_cmp cmp) {
  struct heap* h = malloc(sizeof(struct heap));

  if (!h) {
    errno = ENOMEM;
    return NULL;
  }

  h->cmp = cmp;
  h->cap = cap;
  h->len = 0;
  h->dat = malloc(sizeof(void*) * (cap + 1));

  memset(h->dat, 0, sizeof(void*) * cap);

  if (!h->dat) {
    errno = ENOMEM;
    free(h);
    return NULL;
  }

  return h;
}

int heap_fixd(struct heap* h, int i) {
  int l = HEAP_L(i);
  int r = HEAP_R(i);

  int m;

  if (l < h->len && h->cmp(h->dat[l], h->dat[i]) == -1)
    m = l;
  else
    m = i;

  if (r < h->len && h->cmp(h->dat[r], h->dat[m]) == -1)
    m = r;

  if (m != i) {
    void* t = h->dat[i];

    h->dat[i] = h->dat[m];
    h->dat[m] = t;

    return heap_fixd(h, m);
  }

  return 0;
}

int heap_fixu(struct heap *h, int i) {
  if (i == 0)
    return 0;

  int p = HEAP_P(i);

  if (h->cmp(h->dat[p], h->dat[i]) == 1) {
    void *t = h->dat[i];

    h->dat[i] = h->dat[p];
    h->dat[p] = t;

    return heap_fixu(h, p);
  }

  return 0;
}

int heap_ins(struct heap* h, void* e) {
  if (h->len == h->cap)
    return -1;

  h->len += 1;
  h->dat[h->len] = e;
  
  return heap_fixu(h, h->len);
}

int heap_ext(struct heap* h, void** e) {
  if (h->len == 0) {
    *e = NULL;
    return 0;
  }

  *e = h->dat[1];
  h->dat[1] = h->dat[h->len];
  h->len -= 1;

  return heap_fixd(h, 1);
}

int heap_cls(struct heap* h) {
  free(h->dat);
  free(h);

  return 0;
}
