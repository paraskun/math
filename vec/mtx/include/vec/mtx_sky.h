#ifndef SMTX_H
#define SMTX_H

#include <vec/vec.h>

#include <stddef.h>
#include <stdio.h>

struct mtx_sky {
  struct {
    int n;
    int s;
  } pps;

  double* dv;
  double* lv;
  double* uv;

  int* p;
};

struct mtx_sky* mtx_sky_new(int n, int s);

int mtx_sky_fget(FILE* f, struct mtx_sky* ap);
int mtx_sky_fput(FILE* f, struct mtx_sky* ap);

int mtx_sky_ldu(struct mtx_sky* ap);

int mtx_sky_hlb(struct mtx_sky* ap);

int mtx_sky_vmlt(struct mtx_sky* ap, struct vec* xp, struct vec* fp);

int mtx_sky_cls(struct mtx_sky* ap);

#endif  // SMTX_H
