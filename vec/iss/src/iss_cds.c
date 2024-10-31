#include <iss_cds.h>

static int step(struct mtx_cds* mp, struct vec* xp, struct vec* fp,
                struct vec* rp, double omg) {
  int n = mp->n;
  int c = mp->c;
  int d = mp->d;

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

int iss_cds_jac_solve(struct mtx_cds* mp, struct vec* xp, struct vec* fp,
                      struct iss_jac_pps pps, void (*f)(int, double)) {
  struct vec* tp = vec_new(xp->n);

  double nfp = 0;

  vec_nrm(fp, &nfp);

  double ntp = 0;
  double res = 1;
  double eps = pps.pps.eps;
  double omg = pps.omg;

  int ms = pps.pps.ms;

  for (int s = 0; s < ms && res >= eps; ++s) {
    step(mp, xp, fp, tp, omg);
    swap(xp, tp);

    mtx_cds_vmlt(mp, xp, tp);
    vec_sub(fp, tp, tp);
    vec_nrm(tp, &ntp);

    res = ntp / nfp;

    f(s, res);
  }

  vec_free(tp);
  return 0;
}

int iss_cds_rlx_solve(struct mtx_cds* mp, struct vec* xp, struct vec* fp,
                      struct iss_jac_pps pps, void (*f)(int, double)) {
  struct vec* tp = vec_new(xp->n);

  double nfp = 0;

  vec_nrm(fp, &nfp);

  double ntp = 0;
  double res = 1;
  double eps = pps.pps.eps;
  double omg = pps.omg;

  int ms = pps.pps.ms;

  for (int s = 0; s < ms && res >= eps; ++s) {
    step(mp, xp, fp, xp, omg);

    mtx_cds_vmlt(mp, xp, tp);
    vec_sub(fp, tp, tp);
    vec_nrm(tp, &ntp);

    res = ntp / nfp;

    f(s, res);
  }

  vec_free(tp);
  return 0;
}
