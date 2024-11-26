#ifndef FEM_H
#define FEM_H

#include <fem/hex.h>

#include <mtx/mtx.h>
#include <mtx/csj.h>

struct fem {
  int vs;
  int hs;
  int fs;

  struct vtx** vtx;
  struct hex** hex;
  struct fce** fce;

  struct mtx_csj* a;
  struct vec* b;
};

struct fem* fem_new();

int fem_get(FILE* obj, struct fem* fem);
int fem_evo(struct fem* fem);
int fem_asm(struct fem* fem);
int fem_slv(struct fem* fem, struct vec* q);
int fem_cls(struct fem* fem);

#endif  // FEM_H
