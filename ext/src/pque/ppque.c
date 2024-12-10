#include <ext/pque.h>

#include <errno.h>
#include <stdlib.h>

#include "pque.h"

struct ppque {
  uint len;
  uint cap;
  bool cov;

  int (*cmp)(void*, void*);
  int (*anc)(void*, uint);

  void** data;
};

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

  if (q->anc) {
    q->anc(q->data[a], a);
    q->anc(q->data[b], b);
  }
}

int ppque_ini(struct ppque** h) {
  if (!h) {
    errno = EINVAL;
    return -1;
  }

  struct ppque* que = malloc(sizeof(struct ppque));

  if (!que) {
    errno = ENOMEM;
    return -1;
  }

  que->cap = 0;
  que->len = 0;
  que->cmp = &cmp;
  que->anc = nullptr;
  que->cov = false;
  que->data = nullptr;

  *h = que;

  return 0;
}

int ppque_cls(struct ppque** h) {
  if (!h || !(*h)) {
    errno = EINVAL;
    return -1;
  }

  if (!(*h)->cov)
    free((*h)->data);

  free(*h);

  return 0;
}

int ppque_new(struct ppque* q, uint cap) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  q->cap = cap;
  q->len = 0;
  q->cov = false;
  q->data = malloc(sizeof(void*) * (cap + 1));

  if (!q->data) {
    errno = ENOMEM;
    return -1;
  }

  return 0;
}

int ppque_cov(struct ppque* q, void** s, uint cap) {
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

int ppque_cmp(struct ppque* q, int (*cmp)(void*, void*)) {
  if (!q || !cmp) {
    errno = EINVAL;
    return -1;
  }

  q->cmp = cmp;

  return 0;
}

int ppque_anc(struct ppque* q, int (*anc)(void*, uint)) {
  if (!q || !anc) {
    errno = EINVAL;
    return -1;
  }

  q->anc = anc;

  return 0;
}

uint ppque_cap(struct ppque* q) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  return q->cap;
}

uint ppque_len(struct ppque* q) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  return q->len;
}

int ppque_fix(struct ppque* q) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  for (uint i = q->len / 2; i > 0; --i)
    if (ppque_fixd(q, i))
      return -1;

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

  if (l <= q->len && q->cmp(q->data[l], q->data[i]) == 1)
    m = l;
  else
    m = i;

  if (r <= q->len && q->cmp(q->data[r], q->data[m]) == 1)
    m = r;

  if (m != i) {
    swap(q, m, i);

    return ppque_fixd(q, m);
  }

  return 0;
}

int ppque_ins(struct ppque* q, void* e) {
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

  if (q->anc)
    q->anc(e, q->len);

  return ppque_fixu(q, q->len);
}

int ppque_ext(struct ppque* q, void** e) {
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

  if (q->anc)
    q->anc(q->data[1], 1);

  return ppque_fixd(q, 1);
}

int ppque_rst(struct ppque* q) {
  if (!q) {
    errno = EINVAL;
    return -1;
  }

  q->len = q->cap;

  return 0;
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
