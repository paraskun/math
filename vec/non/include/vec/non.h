#ifndef NON_H
#define NON_H

#include <vec/vec.h>

struct non_itr {
  uint k;

  double del;
  double err;

  struct vec* x;
};

typedef void (*fun_non_cbk)(struct non_itr*);

struct non_pps {
  uint hem;
  double eps;
  double hop;
  fun_non_cbk cbk;
  struct non_itr* res;
};

int non_new_slv(uint m, double (**f)(struct vec*), struct vec* x, struct non_pps pps);

#endif  // NON_H
