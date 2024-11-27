#ifndef SMTX_H
#define SMTX_H

#include <vec/vec.h>

#include <stddef.h>
#include <stdio.h>

struct mtx_sky {
  int n;
  int s;

  double* dv;
  double* lv;
  double* uv;

  int* p;
};

struct mtx_sky* mtx_sky_new(int n, int s);

void mtx_sky_fget(FILE* f, struct mtx_sky* mp);

void mtx_sky_fput(FILE* f, struct mtx_sky* mp);

void mtx_sky_ldu(struct mtx_sky* mp);

void mtx_sky_ddm(struct mtx_sky* mp, int k);

void mtx_sky_hlb(struct mtx_sky* mp);

void mtx_sky_vmlt(struct mtx_sky* ap, struct vec* xp, struct vec* rp);

void mtx_sky_free(struct mtx_sky* mp);

#endif  // SMTX_H
