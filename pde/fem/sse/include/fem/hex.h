#ifndef FEM_HEX_H
#define FEM_HEX_H

#include <fem/fce.h>
#include <fem/vtx.h>
#include <mtx/mtx.h>
#include <mtx/csj.h>

struct hex {
  int vtx[8];

  struct {
    double lam;
    double gam;
  } pps;

  struct {
    struct mtx* g;
    struct mtx* m;
    struct vec* b;
  } dep;
};

struct hex* hex_new();

int hex_get(FILE* obj, struct hex* h);
int hex_evo(struct hex* h, struct vtx** v);
int hex_mov(struct hex* h, struct mtx_csj* a, struct vec* b);
int hex_cls(struct hex* h);

#endif  // FEM_HEX_H
