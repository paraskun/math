#ifndef MTX_CSR_H
#define MTX_CSR_H

#include <vec/vec.h>

#include <stdio.h>

struct mtx_csj_pps {
  int n;
  int ne;
};

struct mtx_csj {
  struct mtx_csj_pps pps;

  double* dr;
  double* lr;
  double* ur;

  int* ia;
  int* ja;
};

struct mtx_csj_pkt {
  FILE* pps;

  FILE* dr;
  FILE* lr;
  FILE* ur;

  FILE* ia;
  FILE* ja;
};

struct mtx_csj* mtx_csj_new(struct mtx_csj_pps pps);

int mtx_csj_put(struct mtx_csj_pkt* pkt, struct mtx_csj* mp);
int mtx_csj_get(struct mtx_csj_pkt* pkt, struct mtx_csj* mp);

int mtx_csj_ilu(struct mtx_csj* mp, struct mtx_csj* rp);
int mtx_csj_dgl(struct mtx_csj* mp, struct mtx_csj* rp);

int mtx_csj_all(struct mtx_csj* mp);
int mtx_csj_ddm(struct mtx_csj* mp, int k);
int mtx_csj_hlb(struct mtx_csj* mp);

int mtx_csj_vmlt(
    struct mtx_csj* mp, 
    struct vec* xp, 
    struct vec* fp);

void mtx_csj_cls(struct mtx_csj* mp);
void mtx_csj_pkt_cls(struct mtx_csj_pkt* pkt);

#endif  // MTX_CSR_H
