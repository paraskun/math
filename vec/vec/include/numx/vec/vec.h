#ifndef NUMX_VEC_H
#define NUMX_VEC_H

#include <float.h>

typedef unsigned int uint;

struct vec {
  uint n;

  double* dat;
};

#define vec_new(...) vec_new_var(__VA_ARGS__, DBL_MAX)

int vec_new_var(struct vec** v, uint n, ...);
int vec_cls(struct vec** h);

int vec_cmb(struct vec* a, struct vec* b, struct vec* r, double k);
int vec_crs(struct vec* a, struct vec* b, struct vec* r);
int vec_dot(struct vec* a, struct vec* b, double* r);
int vec_nrm(struct vec* v, double* r);

int vec_cpy(struct vec* s, struct vec* d);
int vec_rst(struct vec* v);

#endif  // NUMX_VEC_H
