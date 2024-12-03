#include <vec/iss_cds.h>

static int step(
    struct mtx_cds* mp, 
    struct vec* xp, 
    struct vec* fp,
    struct vec* rp, 
    double omg) {
  int n = mp->pps.n;
  int c = mp->pps.c;
  int d = mp->pps.d;

  double* fvp = fp->vp;
  double* xvp = xp->vp;
  double* rvp = rp->vp;

  int* la = mp->la;
  double** ad = mp->ad;

  for (int i = 0; i < n; ++i) {
    double pi = fvp[i];

    for (int k = 0; k < c; ++k) {
      int j = la[k] + i;

      if (j < 0 || j >= n)
        continue;

      pi -= ad[i][k] * xvp[j];
    }

    rvp[i] = xvp[i] + (omg / ad[i][d]) * pi;
  }

  return 0;
}

static inline void swap(struct vec* ap, struct vec* bp) {
  double* tvp = ap->vp;

  ap->vp = bp->vp;
  bp->vp = tvp;
}

int iss_cds_jac_slv(
    struct mtx_cds* mp, 
    struct vec* xp, 
    struct vec* fp,
    struct iss_jac_pps* pps, 
    struct iss_res* res,
    fun_iss_cbk cbk) {
  struct vec* tp = vec_new(xp->n);

  double nfp = 0;
  double ntp = 0;
  double rsd = 1;
  double eps = pps->pps.eps;
  double omg = pps->omg;

  int mk = pps->pps.mk;

  vec_nrm(fp, &nfp);

  for (int k = 0; k < mk && rsd >= eps; ++k) {
    step(mp, xp, fp, tp, omg);
    swap(xp, tp);

    mtx_cds_vmlt(mp, xp, tp);
    vec_cmb(fp, tp, tp, -1);
    vec_nrm(tp, &ntp);

    rsd = ntp / nfp;

    res->k = k;
    res->res = rsd;

    if (cbk)
      cbk(res);
  }

  vec_cls(tp);
  return 0;
}

int iss_cds_rlx_slv(
    struct mtx_cds* mp, 
    struct vec* xp, 
    struct vec* fp,
    struct iss_jac_pps* pps, 
    struct iss_res* res,
    fun_iss_cbk cbk) {
  struct vec* tp = vec_new(xp->n);

  double nfp = 0;
  double ntp = 0;
  double rsd = 1;
  double eps = pps->pps.eps;
  double omg = pps->omg;

  int mk = pps->pps.mk;

  vec_nrm(fp, &nfp);

  for (int k = 0; k < mk && rsd >= eps; ++k) {
    step(mp, xp, fp, xp, omg);

    mtx_cds_vmlt(mp, xp, tp);
    vec_cmb(fp, tp, tp, -1);
    vec_nrm(tp, &ntp);

    rsd = ntp / nfp;

    res->k = k;
    res->res = rsd;

    if (cbk)
      cbk(res);
  }

  vec_cls(tp);
  return 0;
}
