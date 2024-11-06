#ifndef MTX_CSR_H
#define MTX_CSR_H

#include <vec.h>

#include <stdio.h>

struct mtx_csr {
  int n;
  int ne;

  double* ar;

  int* ia;
  int* ja;
};

struct mtx_csr* mtx_csr_new(int n, int ne);

int mtx_csr_fget(FILE* f, struct mtx_csr* mp);
int mtx_csr_fput(FILE* f, struct mtx_csr* mp, int pty);

int mtx_csr_ilu(struct mtx_csr* mp, struct mtx_csr* rp);

int mtx_csr_vmlt(struct mtx_csr* mp, struct vec* xp, struct vec* fp);

void mtx_csr_free(struct mtx_csr* mp);

#endif  // MTX_CSR_H
