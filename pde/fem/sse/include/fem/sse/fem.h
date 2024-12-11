#ifndef FEM_H
#define FEM_H

#include <fem/sse/hex.h>

#include <vec/mtx.h>
#include <vec/mtx_csj.h>

#include <ext/type.h>

struct fem {
  int vs;
  int hs;
  int fs;

  struct vtx** vtx;
  struct hex** hex;
  struct fce** fce;

  struct {
    double (**fun)(struct vtx*);
  } pps;

  struct mtx_csj* a;
  struct vec* b;
};

int fem_ini(struct fem** h);
int fem_cls(struct fem** h);

int fem_fget(struct fem* fem, FILE* f);

int fem_evo(struct fem* fem);
int fem_asm(struct fem* fem);
int fem_slv(struct fem* fem, struct vec* q);
int fem_get(struct fem* fem, struct vec* q, struct vtx* v, uint h, double* r);

#endif  // FEM_H
