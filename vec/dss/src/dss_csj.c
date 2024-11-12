#include <dss_csj.h>

#include <string.h>

int iss_csj_ilu_lslv(struct mtx_csj* mp, struct vec* xp, struct vec* fp) {
  int n = mp->pps.n;

  int* mil = mp->il;
  int* mjl = mp->jl;

  double* mdr = mp->dr;
  double* mlr = mp->lr;

  double* xv = xp->vp;
  double* fv = fp->vp;

  for (int i = 0; i < n; ++i) {
    int lr0 = mil[i];
    int lr1 = mil[i + 1];

    double s = 0;

    for (int lr = lr0; lr < lr1; ++lr) {
      int j = mjl[lr];

      s += mlr[lr] * xv[j];
    }

    xv[i] = (fv[i] - s) / mdr[i];
  }

  return 0;
}

int iss_csj_ilu_uslv(struct mtx_csj* mp, struct vec* xp, struct vec* fp) {
  int n = mp->pps.n;

  int* mju = mp->ju;
  int* miu = mp->iu;

  double* mdr = mp->dr;
  double* mur = mp->ur;

  double* xv = xp->vp;
  double* fv = fp->vp;

  memcpy(xv, fv, sizeof(double) * n);

  for (int j = n - 1; j > -1; --j) {
    xv[j] /= mdr[j];

    int ur0 = mju[j];
    int ur1 = mju[j + 1];

    double x = xv[j];

    for (int ur = ur0; ur < ur1; ++ur) {
      int ui = miu[ur];

      xv[ui] -= mur[ur] * x;
    }
  }

  return 0;
}

int iss_csj_dgl_slv(struct mtx_csj* mp, struct vec* xp, struct vec* fp) {
  int n = mp->pps.n;

  double* mdr = mp->dr;
  double* xvp = xp->vp;
  double* fvp = fp->vp;

  for (int i = 0; i < n; ++i)
    xvp[i] = fvp[i] / mdr[i];

  return 0;
}
