#include <dsa/sll.h>

#include <errno.h>
#include <stdlib.h>

static int cmp(void* a, void* b) {
  if (a > b)
    return -1;

  if (a < b)
    return 1;

  return 0;
}

int psll_new(struct psll* l) {
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

int psll_add(struct psll* l, void* e) {
  if (!l) {
    errno = EINVAL;
    return -1;
  }

  struct pslln* n = NULL;

  if (l->len == 0)
    return psll_ins(l, &n, e);

  n = l->beg;

  while (n->next && l->cmp(n->next->e, e) == 1)
    n = n->next;

  return psll_ins(l, &n, e);
}

int psll_ins(struct psll* l, struct pslln** h, void* e) {
  if (!l) {
    errno = EINVAL;
    return -1;
  }

  struct pslln* n = malloc(sizeof(struct pslln));

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

int psll_srh(struct psll* l, struct pslln** h, void* e) {
  if (!l) {
    errno = EINVAL;
    return -1;
  }

  struct pslln* n = l->beg;

  while (n && l->cmp(n->e, e) != 0)
    n = n->next;

  *h = n;

  return 0;
}

int psll_cls(struct psll* l) {
  if (!l) {
    errno = EINVAL;
    return -1;
  }

  struct pslln* n = l->beg;
  struct pslln* t = NULL;

  while (n) {
    t = n;
    n = n->next;

    free(t);
  }

  return 0;
}
