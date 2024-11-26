#ifndef FEM_SLL_H
#define FEM_SLL_H

struct lln {
  int e;

  struct lln* next;
};

struct sll {
  int (*cmp)(int, int);

  struct lln* beg;
  struct lln* end;
};

int sll_ini(struct sll* l);
int sll_ins(struct sll* l, int e);
int sll_cls(struct sll* l);

#endif  // FEM_SLL_H
