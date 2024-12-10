#include <ext/sll.h>

#include <errno.h>
#include <stdlib.h>

struct psln {
  void* e;

  struct psln* next;
};

struct psll {
  uint len;
  bool ctl;

  int (*cmp)(void*, void*);

  struct psln* beg;
  struct psln* end;
};

int psll_ini(struct psll** h) {
  if (!h) {
    errno = EINVAL;
    return -1;
  }

  struct psll* l = malloc(sizeof(struct psll));

  if (!l) {
    errno = ENOMEM;
    return -1;
  }

  l->len = 0;
  l->ctl = false;
  l->cmp = nullptr;
  l->beg = nullptr;
  l->end = nullptr;

  *h = l;

  return 0;
}

int psll_cls(struct psll** h) {
  if (!h || !(*h)) {
    errno = EINVAL;
    return -1;
  }
  struct psll* l = *h;
  struct psln* i = l->beg;
  struct psln* t;

  while (i) {
    t = i;
    i = i->next;

    if (l->ctl)
      free(t->e);

    free(t);
  }

  free(l);

  return 0;
}

int psll_cmp(struct psll* l, int (*cmp)(void*, void*)) {
  if (!l) {
    errno = EINVAL;
    return -1;
  }

  l->cmp = cmp;

  return 0;
}

int psll_ctl(struct psll* l, bool ctl) {
  if (!l) {
    errno = EINVAL;
    return false;
  }

  l->ctl = ctl;

  return 0;
}

int psll_next(struct psll* l, struct psln** i) {
  if (!l || !i) {
    errno = EINVAL;
    return -1;
  }

  *i = *i ? (*i)->next : l->beg;

  return *i ? 1 : 0;
}

uint psll_len(struct psll* l) {
  if (!l) {
    errno = EINVAL;
    return -1;
  }

  return l->len;
}

int psll_add(struct psll* l, void* e) {
  if (!l) {
    errno = EINVAL;
    return -1;
  }

  struct psln* n = nullptr;

  if (l->len == 0)
    return psll_ins(l, &n, e);

  if (!l->cmp)
    return psll_ins(l, &l->end, e);

  n = l->beg;

  while (n->next && l->cmp(n->next->e, e) == 1)
    n = n->next;

  return psll_ins(l, &n, e);
}

int psll_ins(struct psll* l, struct psln** h, void* e) {
  if (!l || !h) {
    errno = EINVAL;
    return -1;
  }

  struct psln* p = *h;
  struct psln* n = malloc(sizeof(struct psln));

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

int psll_pop(struct psll* l, struct psln** h, void** e) {
  if (!l || !h) {
    errno = EINVAL;
    return -1;
  }

  struct psln* p = *h;
  struct psln* n;

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

int psll_get(struct psln* n, void** e) {
  if (!n || !e) {
    errno = EINVAL;
    return -1;
  }

  *e = n->e;

  return 0;
}

int psll_srh(struct psll* l, struct psln** h, void* e) {
  if (!l) {
    errno = EINVAL;
    return -1;
  }

  *h = nullptr;

  while (psll_next(l, h)) {
    if (l->cmp((*h)->e, e) == 0)
      return 0;

    if (l->cmp && l->cmp((*h)->e, e) == -1)
      break;
  }

  *h = nullptr;

  return 0;
}
