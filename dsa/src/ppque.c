#include <dsa/pque.h>

#include <errno.h>
#include <stdlib.h>

static int cmp(void* a, void* b) {
  if (a > b)
    return -1;

  if (a < b)
    return 1;

  return 0;
}

static inline void swap(struct ppque* q, uint a, uint b) {
  void* t = q->data[a];

  q->data[a] = q->data[b];
  q->data[b] = t;
}

int ppque_new(struct ppque* q, uint cap) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  q->cap = cap;
  q->len = 0;
  q->cmp = &cmp;
  q->data = malloc(sizeof(void*) * (cap + 1));

  if (!q->data) {
    errno = ENOMEM;
    return -1;
  }

  return 0;
}

int ppque_fix(struct ppque* q) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  for (uint i = q->len / 2; i > 0; --i)
    ppque_fixd(q, i);

  return 0;
}

int ppque_fixu(struct ppque* q, uint i) {
  if (i < 1 || i > q->len) {
    errno = EINVAL;
    return -1;
  }

  uint p = PQUE_P(i);

  while (i > 1 && q->cmp(q->data[p], q->data[i]) == -1) {
    swap(q, p, i);

    i = p;
    p = PQUE_P(i);
  }

  return 0;
}

int ppque_fixd(struct ppque* q, uint i) {
  if (i < 1 || i > q->len) {
    errno = EINVAL;
    return -1;
  }

  uint l = PQUE_L(i);
  uint r = PQUE_R(i);

  uint m;

  if (l < q->len && q->cmp(q->data[l], q->data[i]) == 1)
    m = l;
  else
    m = i;

  if (r < q->len && q->cmp(q->data[r], q->data[m]) == 1)
    m = r;

  if (m != i) {
    swap(q, m, i);

    return ppque_fixd(q, m);
  }

  return 0;
}

int ppque_ins(struct ppque* q, void* e) {
  if (q->len == q->cap) {
    errno = EDQUOT;
    return -1;
  }

  q->len += 1;
  q->data[q->len] = e;

  return ppque_fixu(q, q->len);
}

int ppque_ext(struct ppque* q, void** e) {
  if (q->len == 0) {
    errno = ENOENT;
    return -1;
  }

  *e = q->data[1];
  q->data[1] = q->data[q->len];
  q->data[q->len] = *e;
  q->len -= 1;

  return ppque_fixd(q, 1);
}

int ppque_srt(struct ppque* q) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  for (uint i = q->len; i > 1; --i) {
    swap(q, 1, i);
    q->len -= 1;
    ppque_fixd(q, 1);
  }

  return 0;
}

int ppque_cls(struct ppque* q) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  free(q->data);

  return 0;
}
