#ifndef VEC_H
#define VEC_H

#include <stdio.h>

struct vec {
  int n;
  double* vp;
};

struct vec* vec_new(int n);

int vec_seq(struct vec* vp, int s);

int vec_get(FILE* f, struct vec* vp);
int vec_put(FILE* f, struct vec* vp);

int vec_cmb(struct vec* ap, struct vec* bp, struct vec* rp, double k);
int vec_mlt(struct vec* ap, struct vec* bp, double* rp);
int vec_nrm(struct vec* vp, double* rp);
int vec_cpy(struct vec* src, struct vec* dst);
int vec_zer(struct vec* vp);

int vec_cls(struct vec* vp);

#endif  // VEC_H
