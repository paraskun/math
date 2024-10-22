#ifndef PMTX_H
#define PMTX_H

#include <vec.h>

#include <stdio.h>

struct mtx_pie {
  int n;
  int sr;
  int rc;

  double* vp;
};

struct mtx_pie* mtx_pie_new(int n, int sr, int rc);

int mtx_pie_fget(FILE* f, struct mtx_pie* mp);
int mtx_pie_fput(FILE* f, struct mtx_pie* mp);

int mtx_pie_seq(struct mtx_pie* mp, int s);

int mtx_pie_vmlt(struct mtx_pie* mp, struct vec* xp, struct vec* bp);

int mtx_pie_free(struct mtx_pie* mp);

#endif  // PMTX_H
