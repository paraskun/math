#ifndef MTX_H
#define MTX_H

#include <def.h>
#include <vec.h>

#include <stdio.h>

struct mtx;

void mtx_fget(FILE* f, struct mtx* a);
void mtx_fput(FILE* f, struct mtx* a);

void mtx_free(struct mtx* a);

#endif  // MTX_H
