#include <dss_csj.h>

#include <string.h>

int iss_csj_ilu_lslv(
    struct mtx_csj* mp, 
    struct vec* xp, 
    struct vec* fp) {
  int n = mp->pps.n;

  int* mia = mp->ia;
  int* mja = mp->ja;

  double* mdr = mp->dr;
  double* mlr = mp->lr;

  double* xv = xp->vp;
  double* fv = fp->vp;

  for (int i = 0; i < n; ++i) {
    int lr0 = mia[i];
    int lr1 = mia[i + 1];

    double s = 0;

    for (int lr = lr0; lr < lr1; ++lr) {
      int j = mja[lr];

      s += mlr[lr] * xv[j];
    }

    xv[i] = (fv[i] - s) / mdr[i];
  }

  return 0;
}

int iss_csj_ilu_uslv(
    struct mtx_csj* mp, 
    struct vec* xp, 
    struct vec* fp) {
  int n = mp->pps.n;

  int* mia = mp->ia;
  int* mja = mp->ja;

  double* mdr = mp->dr;
  double* mur = mp->ur;

  double* xv = xp->vp;
  double* fv = fp->vp;

  memcpy(xv, fv, sizeof(double) * n);

  for (int j = n - 1; j > -1; --j) {
    xv[j] /= mdr[j];

    int ur0 = mia[j];
    int ur1 = mia[j + 1];

    double x = xv[j];

    for (int ur = ur0; ur < ur1; ++ur) {
      int ui = mja[ur];

      xv[ui] -= mur[ur] * x;
    }
  }

  return 0;
}

int iss_csj_dgl_slv(
    struct mtx_csj* mp, 
    struct vec* xp, 
    struct vec* fp) {
  int n = mp->pps.n;

  double* dr = mp->dr;
  double* xv = xp->vp;
  double* fv = fp->vp;

  for (int i = 0; i < n; ++i)
    xv[i] = fv[i] / dr[i];

  return 0;
}
