#include <ext/pque.h>

#include <errno.h>
#include <stdlib.h>

#include "pque.h"

struct upque {
  uint len;
  uint cap;
  bool cov;

  int (*cmp)(uint, uint);

  uint* data;
};

static int cmp(uint a, uint b) {
  if (a > b)
    return -1;

  if (a < b)
    return 1;

  return 0;
}

static inline void swap(struct upque* q, uint a, uint b) {
  uint t = q->data[a];

  q->data[a] = q->data[b];
  q->data[b] = t;
}

int upque_ini(struct upque** h) {
  if (!h) {
    errno = EINVAL;
    return -1;
  }

  struct upque* que = malloc(sizeof(struct upque));

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

int upque_cls(struct upque** h) {
  if (!h || !(*h)) {
    errno = EINVAL;
    return -1;
  }

  if (!(*h)->cov)
    free((*h)->data);

  free(*h);

  return 0;
}

int upque_new(struct upque* q, uint cap) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  q->cap = cap;
  q->len = 0;
  q->cov = false;
  q->data = malloc(sizeof(uint) * (cap + 1));

  if (!q->data) {
    errno = ENOMEM;
    return -1;
  }

  return 0;
}

int upque_cov(struct upque* q, uint* s, uint cap) {
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

int upque_cmp(struct upque* q, int (*cmp)(uint, uint)) {
  if (!q || !cmp) {
    errno = EINVAL;
    return -1;
  }

  q->cmp = cmp;

  return 0;
}

uint upque_cap(struct upque* q) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  return q->cap;
}

uint upque_len(struct upque* q) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  return q->len;
}

int upque_fix(struct upque* q) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  for (uint i = q->len / 2; i > 0; --i)
    if (upque_fixd(q, i))
      return -1;

  return 0;
}

int upque_fixu(struct upque* q, uint i) {
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

int upque_fixd(struct upque* q, uint i) {
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

    return upque_fixd(q, m);
  }

  return 0;
}

int upque_ins(struct upque* q, uint e) {
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

  return upque_fixu(q, q->len);
}

int upque_ext(struct upque* q, uint* e) {
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

  return upque_fixd(q, 1);
}

int upque_rst(struct upque* q) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  q->len = q->cap;

  return 0;
}

int upque_srt(struct upque* q) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  for (uint i = q->len; i > 1; --i) {
    swap(q, 1, i);
    q->len -= 1;
    upque_fixd(q, 1);
  }

  return 0;
}
