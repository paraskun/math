#ifndef MTX_CSR_H
#define MTX_CSR_H

#include <vec/vec.h>

#include <stdio.h>

struct mtx_csj {
  struct {
    int n;
    int ne;
  } pps;

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

struct mtx_csj* mtx_csj_new(int n, int ne);

int mtx_csj_fput(struct mtx_csj_pkt* pkt, struct mtx_csj* mp);
int mtx_csj_fget(struct mtx_csj_pkt* pkt, struct mtx_csj* mp);

int mtx_csj_ilu(struct mtx_csj* ap, struct mtx_csj* bp);
int mtx_csj_dgl(struct mtx_csj* ap, struct mtx_csj* bp);

int mtx_csj_hlb(struct mtx_csj* ap);

int mtx_csj_vmlt(struct mtx_csj* ap, struct vec* xp, struct vec* fp);

void mtx_csj_cls(struct mtx_csj* mp);
void mtx_csj_pkt_cls(struct mtx_csj_pkt* pkt);

#endif  // MTX_CSR_H
