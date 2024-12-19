#ifndef VEC_H
#define VEC_H

typedef unsigned int uint;

struct vec {
  uint dim;

  double* data;
};

int vec_new(struct vec** h, uint cap);
int vec_cls(struct vec** h);

int vec_cmb(struct vec* a, struct vec* b, struct vec* r, double k);
int vec_crs(struct vec* a, struct vec* b, struct vec* r);
int vec_dot(struct vec* a, struct vec* b, double* r);
int vec_nrm(struct vec* v, double* r);

int vec_cpy(struct vec* s, struct vec* d);
int vec_rst(struct vec* v);

#endif  // VEC_H
