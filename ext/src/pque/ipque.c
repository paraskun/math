#include <dsa/pque.h>

#include <errno.h>
#include <stdlib.h>

#include "pque.h"

struct ipque {
  uint len;
  uint cap;
  bool cov;

  int (*cmp)(int, int);

  int* data;
};

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

int ipque_ini(struct ipque** h) {
  if (!h) {
    errno = EINVAL;
    return -1;
  }

  struct ipque* que = malloc(sizeof(struct ipque));

  if (!que) {
    errno = ENOMEM;
    return -1;
  }

  que->cap = 0;
  que->len = 0;
  que->cmp = &cmp;
  que->cov = false;
  que->data = nullptr;

  *h = que;

  return 0;
}

int ipque_cls(struct ipque** h) {
  if (!h || !(*h)) {
    errno = EINVAL;
    return -1;
  }

  if (!(*h)->cov)
    free((*h)->data);

  free(*h);

  return 0;
}

int ipque_new(struct ipque* q, uint cap) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  q->cap = cap;
  q->len = 0;
  q->cov = false;
  q->data = malloc(sizeof(int) * (cap + 1));

  if (!q->data) {
    errno = ENOMEM;
    return -1;
  }

  return 0;
}

int ipque_cov(struct ipque* q, int* s, uint cap) {
  if (!q || !s) {
    errno = EINVAL;
    return -1;
  }

  q->cap = cap;
  q->len = cap;
  q->cov = true;
  q->data = s - 1;

  return 0;
}

int ipque_cmp(struct ipque* q, int (*cmp)(int, int)) {
  if (!q || !cmp) {
    errno = EINVAL;
    return -1;
  }

  q->cmp = cmp;

  return 0;
}

uint ipque_cap(struct ipque* q) {
  if (!q) {
    errno = EINVAL;
    return 0;
  }

  return q->cap;
}

uint ipque_len(struct ipque* q) {
  if (!q) {
    errno = EINVAL;
    return 0;
  }

  return q->len;
}

int ipque_fix(struct ipque* q) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  for (uint i = q->len / 2; i > 0; --i)
    if (ipque_fixd(q, i))
      return -1;

  return 0;
}

int ipque_fixu(struct ipque* q, uint i) {
  if (!q || i < 1 || i > q->len) {
    errno = EINVAL;
    return -1;
  }

  uint p = PQUE_P(i);

  while (i > 1 && q->cmp(q->data[i], q->data[p]) == 1) {
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

  if (l <= q->len && q->cmp(q->data[l], q->data[i]) == 1)
    m = l;
  else
    m = i;

  if (r <= q->len && q->cmp(q->data[r], q->data[m]) == 1)
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

int ipque_rst(struct ipque* q) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  q->len = q->cap;

  return 0;
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


