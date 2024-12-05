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

static inline void swap(struct ipque* q, uint a, uint b) {
  int t = q->data[a];

  q->data[a] = q->data[b];
  q->data[b] = t;
}

int ipque_new(struct ipque* q, uint cap) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  q->cap = cap;
  q->len = 0;
  q->cmp = &cmp;
  q->data = malloc(sizeof(int) * (cap + 1));

  if (!q->data) {
    errno = ENOMEM;
    return -1;
  }

  return 0;
}

int ipque_fix(struct ipque* q) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  q->len = q->cap;

  for (uint i = q->len / 2; i > 0; --i)
    ipque_fixd(q, i);

  return 0;
}

int ipque_fixu(struct ipque* q, uint i) {
  if (!q || i < 1 || i > q->len) {
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

int ipque_fixd(struct ipque* q, uint i) {
  if (!q || i < 1 || i > q->len) {
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

    return ipque_fixd(q, m);
  }

  return 0;
}

int ipque_ins(struct ipque* q, int e) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  if (q->len == q->cap) {
    errno = EDQUOT;
    return -1;
  }

  q->len += 1;
  q->data[q->len] = e;

  return ipque_fixu(q, q->len);
}

int ipque_ext(struct ipque* q, int* e) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  if (q->len == 0) {
    errno = ENOENT;
    return -1;
  }

  *e = q->data[1];
  q->data[1] = q->data[q->len];
  q->data[q->len] = *e;
  q->len -= 1;

  return ipque_fixd(q, 1);
}

int ipque_srt(struct ipque* q) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  for (uint i = q->len; i > 1; --i) {
    swap(q, 1, i);
    q->len -= 1;
    ipque_fixd(q, 1);
  }

  return 0;
}

int ipque_cls(struct ipque* q) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  free(q->data);

  return 0;
}
