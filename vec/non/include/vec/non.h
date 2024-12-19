#ifndef NON_H
#define NON_H

#include <vec/vec.h>

struct non_res {
  double del;
  double err;

  uint k;

  struct vec* x;
};

typedef void (*fun_non_cbk)(struct non_res*);

struct non_pps {
  uint hem;
  double eps;
  double hop;

  struct non_res* res;
  fun_non_cbk cbk;
};

int non_new_slv(uint m, double (**f)(struct vec*), struct vec* x, struct non_pps pps);

#endif  // NON_H
