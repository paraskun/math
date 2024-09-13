#ifndef MTX_H
#define MTX_H

#include <stdint.h>
#include <stdio.h>

#include "../vec/vec.h"

struct mtx;

struct mtx* mtx_new(uint32_t n);
struct mtx* mtx_rnd(uint32_t n, uint32_t u);
struct mtx* mtx_seq(uint32_t n);

void mtx_fget(FILE* f, struct mtx* a);
void mtx_fput(FILE* f, struct mtx* a);
void mtx_cput(struct mtx* a);

void mtx_mmlt(struct mtx* a, struct mtx* b, struct mtx* c);
void mtx_vmlt(struct mtx* a, struct vec* b, struct vec* c);
void mtx_norm(struct mtx* a, double* r);

void mtx_free(struct mtx* a);

#endif  // MTX_H
