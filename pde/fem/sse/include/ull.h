#ifndef FEM_SLL_H
#define FEM_SLL_H

struct lln {
  int e;

  struct lln* next;
};

struct ull {
  struct lln* beg;
  struct lln* end;
};

int ull_ini(struct ull* l);
int ull_ins(struct ull* l, int e);
int ull_cls(struct ull* l);

#endif  // FEM_SLL_H
