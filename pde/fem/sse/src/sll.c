#include <sll.h>

#include <stdlib.h>

int sll_ini(struct sll* l) {
  l->beg = NULL;
  l->end = NULL;

  return 0;
}

int sll_ins(struct sll* l, int e) {
  struct lln* n = malloc(sizeof(struct lln));

  n->e = e;
  n->next = NULL;

  if (l->beg == NULL) {
    l->beg = n;
    l->end = n;

    return 0;
  }

  if (!l->cmp) {
    l->end->next = n;
    l->end = n;

    return 0;
  }

  struct lln* i = l->beg;

  if (l->cmp(e, i->e) != -1) {
    n->next = i;
    l->beg = n;

    return 0;
  }

  while (i->next && l->cmp(i->next->e, e) == 1)
    i = i->next;

  n->next = i->next;
  i->next = n;

  return 0;
}

int sll_cls(struct sll* l) {
  struct lln* i = l->beg;

  while (i) {
    struct lln* n = i->next;
    free(i);
    i = n;
  }

  return 0;
}

int *int_new(int i) {
  int* m = malloc(sizeof(int));
  *m = i;
  return m;
}
