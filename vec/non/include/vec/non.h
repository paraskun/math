#ifndef NON_H
#define NON_H

#include <stdx/fun.h>
#include <vec/mtx.h>
#include <vec/vec.h>

enum non_mod { EXC, CON };

struct non_itr {
  uint k;

  double del;
  double err;

  struct vec* x;
};

struct non_pps {
  enum non_mod mod;
  struct non_itr* itr;

  uint hem;
  double eps;
  double hop;

  struct jmtx* jac;
  struct fun* cbk;
};

int non_new_slv(
  uint m, double (**f)(struct vec*), struct vec* x, struct non_pps pps);

#endif  // NON_H
