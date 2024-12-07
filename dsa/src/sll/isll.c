#include <dsa/sll.h>

#include <errno.h>
#include <stdlib.h>

struct isln {
  int e;

  struct isln* next;
};

struct isll {
  uint len;

  int (*cmp)(int, int);

  struct isln* beg;
  struct isln* end;
};

int isll_ini(struct isll** h) {
  if (!h) {
    errno = EINVAL;
    return -1;
  }

  struct isll* l = malloc(sizeof(struct isll));

  if (!l) {
    errno = ENOMEM;
    return -1;
  }

  l->len = 0;
  l->cmp = nullptr;
  l->beg = nullptr;
  l->end = nullptr;

  *h = l;

  return 0;
}

int isll_cls(struct isll** h) {
  if (!h || !(*h)) {
    errno = EINVAL;
    return -1;
  }
  struct isll* l = *h;
  struct isln* i = l->beg;
  struct isln* t;

  while (i) {
    t = i;
    i = i->next;

    free(t);
  }

  free(l);

  return 0;
}

int isll_cmp(struct isll* l, int (*cmp)(int, int)) {
  if (!l) {
    errno = EINVAL;
    return -1;
  }

  l->cmp = cmp;

  return 0;
}

int isll_next(struct isll* l, struct isln** i) {
  if (!l || !i) {
    errno = EINVAL;
    return -1;
  }

  *i = *i ? (*i)->next : l->beg;

  return *i ? 1 : 0;
}

uint isll_len(struct isll* l) {
  if (!l) {
    errno = EINVAL;
    return -1;
  }

  return l->len;
}

int isll_add(struct isll* l, int e) {
  if (!l) {
    errno = EINVAL;
    return -1;
  }

  struct isln* n = nullptr;

  if (l->len == 0)
    return isll_ins(l, &n, e);

  if (!l->cmp)
    return isll_ins(l, &l->end, e);

  n = l->beg;

  while (n->next && l->cmp(n->next->e, e) == 1)
    n = n->next;

  return isll_ins(l, &n, e);
}

int isll_ins(struct isll* l, struct isln** h, int e) {
  if (!l || !h) {
    errno = EINVAL;
    return -1;
  }

  struct isln* p = *h;
  struct isln* n = malloc(sizeof(struct isln));

  if (!n) {
    errno = ENOMEM;
    return -1;
  }

  n->e = e;

  if (p) {
    n->next = p->next;
    p->next = n;

    if (p == l->end)
      l->end = n;
  } else {
    n->next = l->beg;
    l->beg = n;

    if (l->len == 0)
      l->end = n;

    *h = n;
  }

  l->len += 1;

  return 0;
}

int isll_pop(struct isll* l, struct isln** h, int* e) {
  if (!l || !h) {
    errno = EINVAL;
    return -1;
  }

  struct isln* p = *h;
  struct isln* n;

  if (p) {
    if (!p->next) {
      errno = ENOENT;
      return -1;
    }

    n = p->next;
    p->next = n->next;
    *e = n->e;

    free(n);
    return 0;
  }

  if (!l->beg) {
    errno = ENOENT;
    return -1;
  }

  n = l->beg;
  l->beg = n->next;
  *e = n->e;

  free(n);
  return 0;
}

int isll_get(struct isln* n, int* e) {
  if (!n || !e) {
    errno = EINVAL;
    return -1;
  }

  *e = n->e;

  return 0;
}

int isll_srh(struct isll* l, struct isln** h, int e) {
  if (!l) {
    errno = EINVAL;
    return -1;
  }

  *h = nullptr;

  while (isll_next(l, h)) {
    if (l->cmp((*h)->e, e) == 0)
      return 0;

    if (l->cmp && l->cmp((*h)->e, e) == -1)
      break;
  }

  *h = nullptr;

  return 0;
}
