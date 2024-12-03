#ifndef MTX_CDS_H
#define MTX_CDS_H

#include <vec/vec.h>

#include <stdio.h>

struct mtx_cds {
  struct {
    int n;
    int c;
    int d;
  } pps;

  int* la;
  double** ad;
};

struct mtx_cds* mtx_cds_new(int n, int c);

int mtx_cds_fget(FILE* f, struct mtx_cds* ap);
int mtx_cds_fput(FILE* f, struct mtx_cds* ap);

int mtx_cds_vmlt(struct mtx_cds* ap, struct vec* xp, struct vec* fp);

int mtx_cds_cls(struct mtx_cds* ap);

#endif  // MTX_CDS_H
