#ifndef NUMX_MTX_H
#define NUMX_MTX_H

#include <numx/vec/vec.h>

struct imtx {
  struct ipps {
    uint m;
    uint n;
  } pps;

  double** dat;
};

int imtx_new(struct imtx** h, struct ipps pps);
int imtx_cls(struct imtx** h);

int imtx_vmlt(struct imtx* m, struct vec* v, struct vec* r);
int imtx_mmlt(struct imtx* a, struct imtx* b, struct imtx* r);

struct jmtx {
  struct jpps {
    uint m;
    uint n;
  } pps;

  double (***data)(struct vec*);
};

int jmtx_new(struct jmtx** h, struct jpps pps);
int jmtx_cls(struct jmtx** h);

#define mtx_new(X, p) _Generic((X), \
    struct imtx**: imtx_new,        \
    struct jmtx**: jmtx_new         \
    )(X, p)

#define mtx_cls(X) _Generic((X),  \
    struct imtx**: imtx_cls,      \
    struct jmtx**: jmtx_cls       \
    )(X)

#define mtx_vmlt(X, v, r) _Generic((X), \
    struct imtx*: imtx_vmlt             \
    )(X, v, r)

#define mtx_mmlt(X, b, r) _Generic((X), \
    struct imtx*: imtx_mmlt             \
    )(X, b, r)

#define mtx_evo(X, x, r) _Generic((X), \
    struct jmtx*: jmtx_evo             \
    )(X, x, r)

#endif  // NUMX_MTX_H
