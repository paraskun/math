#ifndef VEC_H
#define VEC_H

#include <stdio.h>

struct vec {
  int n;
  double* vp;
};

struct vec* vec_new(int n);

int vec_seq(struct vec* vp, int s);

int vec_fget(FILE* f, struct vec* vp);
int vec_fput(FILE* f, struct vec* vp);

int vec_add(struct vec* ap, struct vec* bp, struct vec* rp);
int vec_sub(struct vec* ap, struct vec* bp, struct vec* rp);
int vec_cmb(struct vec* ap, struct vec* bp, struct vec* rp, double k);
int vec_mlt(struct vec* ap, struct vec* bp, double* rp);
int vec_nrm(struct vec* vp, double* rp);

void vec_free(struct vec* vp);

#endif  // VEC_H
