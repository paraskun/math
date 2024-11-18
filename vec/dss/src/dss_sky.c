#include <dss_sky.h>

#include <string.h>

static void dss_sky_l(
    struct mtx_sky* mp, 
    struct vec* xp, 
    struct vec* fp) {
  int n = mp->n;
  int* pp = mp->p;

  double* lp = mp->lv;
  double* xvp = xp->vp;
  double* fvp = fp->vp;

  for (int i = 0; i < n; ++i) {
    double sum = 0;

    int k0 = pp[i];
    int k1 = pp[i + 1];
    int j0 = i - k1 + k0;

    for (int k = k0, j = j0; k < k1; ++k, ++j)
      sum += lp[k] * xvp[j];

    xvp[i] = fvp[i] - sum;
  }
}

static void dss_sky_d(
    struct mtx_sky* mp, 
    struct vec* xp, 
    struct vec* fp) {
  int n = mp->n;

  double* dvp = mp->dv;
  double* xvp = xp->vp;
  double* fvp = fp->vp;

  for (int i = 0; i < n; ++i) {
    double e = fvp[i] / dvp[i];
    xvp[i] = e;
  }
}

static void dss_sky_u(
    struct mtx_sky* mp, 
    struct vec* xp, 
    struct vec* fp) {
  int n = mp->n;
  int* pp = mp->p;

  double* up = mp->uv;
  double* xvp = xp->vp;
  double* fvp = fp->vp;

  memcpy(xvp, fvp, sizeof(double) * n);

  for (int j = n - 1; j > 0; --j) {
    int k0 = pp[j];
    int k1 = pp[j + 1];
    int i0 = j - k1 + k0;

    for (int k = k0, i = i0; k < k1; ++k, ++i)
      xvp[i] -= up[k] * xvp[j];
  }
}

void dss_sky_ldu_solve(
    struct mtx_sky* mp, 
    struct vec* xp, 
    struct vec* fp) {
  dss_sky_l(mp, xp, fp);
  dss_sky_d(mp, fp, xp);
  dss_sky_u(mp, xp, fp);
}
