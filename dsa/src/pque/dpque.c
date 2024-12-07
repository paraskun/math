#include <dsa/pque.h>

#include <errno.h>
#include <stdlib.h>

#include "pque.h"

struct dpque {
  uint len;
  uint cap;
  bool cov;

  int (*cmp)(double, double);

  double* data;
};

static int cmp(double a, double b) {
  if (a > b)
    return -1;

  if (a < b)
    return 1;

  return 0;
}

static inline void swap(struct dpque* q, uint a, uint b) {
  double t = q->data[a];

  q->data[a] = q->data[b];
  q->data[b] = t;
}

int dpque_ini(struct dpque** h) {
  if (!h) {
    errno = EINVAL;
    return -1;
  }

  struct dpque* que = malloc(sizeof(struct dpque));

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

int dpque_cls(struct dpque** h) {
  if (!h || !(*h)) {
    errno = EINVAL;
    return -1;
  }

  if (!(*h)->cov)
    free((*h)->data);

  free(*h);

  return 0;
}

int dpque_new(struct dpque* q, uint cap) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  q->cap = cap;
  q->len = 0;
  q->cov = false;
  q->data = malloc(sizeof(double) * (cap + 1));

  if (!q->data) {
    errno = ENOMEM;
    return -1;
  }

  return 0;
}

int dpque_cov(struct dpque* q, double* s, uint cap) {
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

int dpque_cmp(struct dpque* q, int (*cmp)(double, double)) {
  if (!q || !cmp) {
    errno = EINVAL;
    return -1;
  }

  q->cmp = cmp;

  return 0;
}

uint dpque_cap(struct dpque* q) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  return q->cap;
}

uint dpque_len(struct dpque* q) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  return q->len;
}

int dpque_fix(struct dpque* q) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  for (uint i = q->len / 2; i > 0; --i)
    if (dpque_fixd(q, i))
      return -1;

  return 0;
}

int dpque_fixu(struct dpque* q, uint i) {
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

int dpque_fixd(struct dpque* q, uint i) {
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

    return dpque_fixd(q, m);
  }

  return 0;
}

int dpque_ins(struct dpque* q, double e) {
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

  return dpque_fixu(q, q->len);
}

int dpque_ext(struct dpque* q, double* e) {
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

  return dpque_fixd(q, 1);
}

int dpque_rst(struct dpque* q) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  q->len = q->cap;

  return 0;
}

int dpque_srt(struct dpque* q) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  for (uint i = q->len; i > 1; --i) {
    swap(q, 1, i);
    q->len -= 1;
    dpque_fixd(q, 1);
  }

  return 0;
}
