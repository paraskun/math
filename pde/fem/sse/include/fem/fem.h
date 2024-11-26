#ifndef FEM_H
#define FEM_H

#include <fem/hex.h>

#include <mtx_all.h>
#include <mtx_csj.h>

struct fem {
  int vs;
  int hs;

  struct vtx** vtx;
  struct hex** hex;

  struct mtx_csj* a;
  struct vec* b;

  struct fll fll;
};

struct vln {
  int vtx;

  struct vln* next;
};

struct vll {
  struct vln* beg;
};

int vll_add(struct vll* l, int vtx);
int vll_cls(struct vll* l);

struct fem* fem_new();

int fem_get(FILE* obj, struct fem* fem);
int fem_evo(struct fem* fem);
int fem_asm(struct fem* fem);
int fem_slv(struct fem* fem, struct vec* q);
int fem_cls(struct fem* fem);

#endif  // FEM_H
