#ifndef NON_H
#define NON_H

#include <stdx.h>
#include <vec/mtx.h>
#include <vec/vec.h>

enum non_mod { EXC, CON };

struct non_itr {
  uint k;

  double del;
  double err;

  struct vec* x;
};

struct non_opt {
  enum non_mod mod;

  uint   hem;
  double eps;
  double hop;

  struct non_itr* itr;

  struct vcap* cbk;
  struct jmtx* jac;
};

int non_new_slv(struct pcut* f, struct vec* x, struct non_opt opt);

#endif  // NON_H
