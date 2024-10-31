#ifndef MTX_CDS_H
#define MTX_CDS_H

#include <vec.h>

#include <stdio.h>

struct mtx_cds {
  int n;
  int c;
  int d;

  int* la;
  double** ad;
};

struct mtx_cds* mtx_cds_new(int n, int c);

int mtx_cds_fget(FILE* f, struct mtx_cds* mp);
int mtx_cds_fput(FILE* f, struct mtx_cds* mp);

int mtx_cds_vmlt(struct mtx_cds* mp, struct vec* xp, struct vec* fp);

int mtx_cds_ddm(struct mtx_cds* mp, int m, int k);

void mtx_cds_free(struct mtx_cds* mp);

#endif  // MTX_CDS_H
