#ifndef MTX_H
#define MTX_H

#include <vec/vec.h>

struct mtx {
  uint n;

  double** data;
};

int mtx_new(struct mtx** h, uint cap);
int mtx_cls(struct mtx** h);

int mtx_vmlt(struct mtx* a, struct vec* x, struct vec* f);
int mtx_mmlt(struct mtx* a, struct mtx* b, struct mtx* c);

#endif  // MTX_H
