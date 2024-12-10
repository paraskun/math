#ifndef FEM_HEX_H
#define FEM_HEX_H

#include <fem/sse/fce.h>
#include <fem/sse/vtx.h>
#include <vec/mtx.h>
#include <vec/mtx_csj.h>

struct hex {
  int vtx[8];

  struct {
    double (*f)(struct vtx*);

    double lam;
    double gam;
  } pps;

  struct {
    struct mtx* m;
    struct vec* b;
  } loc;
};

int hex_ini(struct hex** h);
int hex_cls(struct hex** h);

int hex_new(struct hex* h);

int hex_sget(struct hex* h, const char* buf, double (**fun)(struct vtx*));

int hex_evo(struct hex* h, struct vtx** v);
int hex_mov(struct hex* h, struct mtx_csj* a, struct vec* b);

#endif  // FEM_HEX_H
