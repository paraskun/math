#ifndef NON_H
#define NON_H

#include <bas/mas.h>
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
  struct non_itr* itr;
  enum non_mod    mod;

  uint   hem;
  double eps;
  double hop;

  struct jmtx* jac;
  struct clsr* cbk;
};

int non_new_slv(struct pcut* f, struct vec* x, struct non_opt opt);

#endif  // NON_H
