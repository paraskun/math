#include <ull.h>

#include <stdlib.h>

int ull_ini(struct ull* l) {
  l->beg = NULL;
  l->end = NULL;

  return 0;
}

int ull_ins(struct ull* l, int e) {
  struct lln* n = malloc(sizeof(struct lln));

  n->e = e;
  n->next = NULL;

  if (l->beg == NULL) {
    l->beg = n;
    l->end = n;

    return 0;
  }

  l->end->next = n;
  l->end = n;

  return 0;
}

int ull_cls(struct ull* l) {
  struct lln* i = l->beg;

  while (i) {
    struct lln* n = i->next;
    free(i);
    i = n;
  }

  return 0;
}
