#ifndef FEM_HEX_H
#define FEM_HEX_H

#include <fem/fce.h>
#include <fem/vtx.h>

struct hex {
  int vtx[8];

  double lam;
  double gam;

  struct mtx* g;
  struct mtx* m;
  struct vec* b;

  struct fll fll;
};

struct hex* hex_new();

int hex_get(FILE* obj, struct hex* h);
int hex_evo(struct hex* h, struct vtx** v, struct fll* l);
int hex_cls(struct hex* h);

#endif  // FEM_HEX_H
