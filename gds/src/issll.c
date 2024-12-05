#include <gds/ssll.h>

#include <errno.h>
#include <stdlib.h>

static int cmp(int a, int b) {
  if (a > b)
    return -1;

  if (a < b)
    return 1;

  return 0;
}

int issll_new(struct issll* l) {
  if (!l) {
    errno = EINVAL;
    return -1;
  }

  l->len = 0;
  l->cmp = &cmp;
  l->beg = NULL;
  l->end = NULL;

  return 0;
}

int issll_add(struct issll* l, int e) {
  if (!l) {
    errno = EINVAL;
    return -1;
  }

  struct islln* n = NULL;

  if (l->len == 0)
    return issll_ins(l, &n, e);

  n = l->beg;

  while (n->next && l->cmp(n->next->e, e) == 1)
    n = n->next;

  return issll_ins(l, &n, e);
}

int issll_ins(struct issll* l, struct islln** h, int e) {
  if (!l) {
    errno = EINVAL;
    return -1;
  }

  struct islln* n = malloc(sizeof(struct islln));

  if (!n) {
    errno = ENOMEM;
    return -1;
  }

  n->e = e;

  if (*h) {
    n->next = (*h)->next;
    (*h)->next = n;

    if (*h == l->end)
      l->end = n;
  } else {
    n->next = l->beg;
    l->beg = n;

    if (l->len == 0)
      l->end = n;
  }

  *h = n;
  l->len += 1;

  return 0;
}

int issll_srh(struct issll* l, struct islln** h, int e) {
  if (!l) {
    errno = EINVAL;
    return -1;
  }

  struct islln* n = l->beg;

  while (n && l->cmp(n->e, e) != 0)
    n = n->next;

  *h = n;

  return 0;
}

int issll_cls(struct issll* l) {
  if (!l) {
    errno = EINVAL;
    return -1;
  }

  struct islln* n = l->beg;
  struct islln* t = NULL;

  while (n) {
    t = n;
    n = n->next;

    free(t);
  }

  return 0;
}
