#ifndef MTX_H
#define MTX_H

#include <vec/vec.h>

struct imtx {
  struct ipps {
    uint dim;
  } pps;

  double** data;
};

int imtx_new(struct imtx** h, struct ipps pps);
int imtx_cls(struct imtx** h);

int imtx_vmlt(struct imtx* m, struct vec* v, struct vec* r);
int imtx_mmlt(struct imtx* a, struct imtx* b, struct imtx* r);

#define mtx_new(X, p) _Generic((X), \
    struct imtx**: imtx_new         \
    )(X, p)

#define mtx_cls(X) _Generic((X),  \
    struct imtx**: imtx_cls       \
    )(X)

#define mtx_vmlt(X, v, r) _Generic((X), \
    struct imtx*: imtx_vmlt             \
    )(X, v, r)

#define mtx_mmlt(X, b, r) _Generic((X), \
    struct imtx*: imtx_mmlt             \
    )(X, b, r)

#endif  // MTX_H
