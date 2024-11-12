#ifndef MTX_CSR_H
#define MTX_CSR_H

#include <vec.h>

#include <stdio.h>

struct mtx_csj_pps {
  int n;
  int le;
  int ue;
};

struct mtx_csj {
  struct mtx_csj_pps pps;

  double* dr;
  double* lr;
  double* ur;

  int* il;
  int* jl;
  int* iu;
  int* ju;
};

struct mtx_csj_pkt {
  FILE* pps;

  FILE* lr;
  FILE* ur;
  FILE* dr;

  FILE* il;
  FILE* jl;
  FILE* iu;
  FILE* ju;
};

struct mtx_csj* mtx_csj_new(struct mtx_csj_pps pps);

int mtx_csj_pps_fget(struct mtx_csj_pkt* pkt, struct mtx_csj_pps* pps);

int mtx_csj_fput(struct mtx_csj_pkt* pkt, struct mtx_csj* mp);
int mtx_csj_fget(struct mtx_csj_pkt* pkt, struct mtx_csj* mp);

int mtx_csj_fput_all(FILE* f, struct mtx_csj* mp);
int mtx_csj_fget_all(FILE* f, struct mtx_csj* mp);

int mtx_csj_ilu(struct mtx_csj* mp, struct mtx_csj* rp);
int mtx_csj_dgl(struct mtx_csj* mp, struct mtx_csj* rp);

int mtx_csj_vmlt(struct mtx_csj* mp, struct vec* xp, struct vec* fp);

void mtx_csj_free(struct mtx_csj* mp);
void mtx_csj_pkt_close(struct mtx_csj_pkt* pkt);

#endif  // MTX_CSR_H
