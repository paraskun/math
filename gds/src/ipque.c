#include <gds/pque.h>

#include <errno.h>
#include <stdlib.h>

static int cmp(int a, int b) {
  if (a > b)
    return -1;

  if (a < b)
    return 1;

  return 0;
}

static inline void swap(struct ipque* q, unsigned a, unsigned b) {
  int t = q->data[a];

  q->data[a] = q->data[b];
  q->data[b] = t;
}

int ipque_new(struct ipque** h, unsigned int cap) {
  struct ipque* q = *h;

  if (!q) {
    q = malloc(sizeof(struct ipque));

    if (!q) {
      errno = ENOMEM;
      return -1;
    }

    q->cap = cap;
    q->len = 0;
    q->cmp = &cmp;
    q->data = malloc(sizeof(int) * (cap + 1));

    if (!q->data) {
      free(q);

      errno = ENOMEM;
      return -1;
    }

    *h = q;

    return 0;
  }

  for (unsigned i = q->len / 2; i > 0; --i)
    ipque_fixd(q, i);

  return 0;
}

int ipque_fixu(struct ipque* q, unsigned i) {
  if (i < 1 || i > q->len) {
    errno = EINVAL;
    return -1;
  }

  unsigned p = PQUE_P(i);

  while (i > 1 && q->cmp(q->data[p], q->data[i]) == -1) {
    swap(q, p, i);

    i = p;
    p = PQUE_P(i);
  }

  return 0;
}

int ipque_fixd(struct ipque* q, unsigned i) {
  if (i < 1 || i > q->len) {
    errno = EINVAL;
    return -1;
  }

  unsigned l = PQUE_L(i);
  unsigned r = PQUE_R(i);

  unsigned m;

  if (l < q->len && q->cmp(q->data[l], q->data[i]) == 1)
    m = l;
  else
    m = i;

  if (r < q->len && q->cmp(q->data[r], q->data[m]) == 1)
    m = r;

  if (m != i) {
    swap(q, m, i);

    return ipque_fixd(q, m);
  }

  return 0;
}

int ipque_ins(struct ipque* q, int e) {
  if (q->len == q->cap) {
    errno = EDQUOT;
    return -1;
  }

  q->len += 1;
  q->data[q->len] = e;

  return ipque_fixu(q, q->len);
}

int ipque_ext(struct ipque* q, int* e) {
  if (q->len == 0) {
    errno = ENOENT;
    return -1;
  }

  *e = q->data[1];
  q->data[1] = q->data[q->len];
  q->len -= 1;

  return ipque_fixd(q, 1);
}

int ipque_cls(struct ipque* q) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  free(q->data);
  free(q);

  return 0;
}
