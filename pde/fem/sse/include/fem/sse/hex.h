#ifndef FEM_HEX_H
#define FEM_HEX_H

#include <fem/sse/fce.h>
#include <fem/sse/vtx.h>
#include <vec/mtx.h>
#include <vec/mtx_csj.h>

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

int hex_get(const char* buf, struct hex* h);
int hex_evo(struct hex* h, struct vtx** v, double (*g)(struct vtx*));
int hex_mov(struct hex* h, struct mtx_csj* a, struct vec* b);
int hex_cls(struct hex* h);

#endif  // FEM_HEX_H
