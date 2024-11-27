#ifndef FEM_H
#define FEM_H

#include <fem/sse/hex.h>

#include <vec/mtx.h>
#include <vec/mtx_csj.h>

#include <sll.h>

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

struct fem* fem_new(double (**fun)(struct vtx*));

int fem_get(FILE* obj, struct fem* fem);
int fem_evo(struct fem* fem);
int fem_asm(struct fem* fem);
int fem_slv(struct fem* fem, struct vec* q);
int fem_cls(struct fem* fem);

#endif  // FEM_H
