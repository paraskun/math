#ifndef VEC_H
#define VEC_H

#include <def.h>
#include <stdio.h>
#include <stdint.h>

struct vec {
  real* v;
  uint32_t n;
};

struct vec* vec_new(uint32_t n);
struct vec* vec_rnd(uint32_t n, uint32_t u);
struct vec* vec_seq(uint32_t n);

void vec_fget(FILE* f, struct vec* a);
void vec_fput(FILE* f, struct vec* a);
void vec_cput(struct vec* a);

void vec_add(struct vec* a, struct vec* b, struct vec* r);
void vec_cmb(struct vec* a, struct vec* b, struct vec* c, double k);
void vec_mlt(struct vec* a, struct vec* b, double* r);
void vec_norm(struct vec* a, double* r);

void vec_free(struct vec* a);

#endif  // VEC_H
