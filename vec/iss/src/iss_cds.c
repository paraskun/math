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
    double xi = fvp[i];

    for (int k = 0; k < c; ++k) {
      int j = la[k] + i;

      if (j < 0 || j >= n)
        continue;

      xi -= ad[i][k] * xvp[j];
    }

    rvp[i] = xvp[i] + (omg / ad[i][d]) * xi;
  }

  return 0;
}

static inline void swap(struct vec* ap, struct vec* bp) {
  double* tvp = ap->vp;
  ap->vp = bp->vp;
  bp->vp = tvp;
}

int iss_cds_jac_solve(struct mtx_cds* mp, struct vec* xp, struct vec* fp,
                      struct props p) {
  struct vec* tp = vec_new(xp->n);

  double nd;
  double nf;

  vec_nrm(fp, &nf);

  for (int k = 0; k < p.mk; ++k) {
    step(mp, xp, fp, tp, p.omg);
    swap(xp, tp);

    mtx_cds_vmlt(mp, xp, tp);

    vec_sub(fp, tp, tp);
    vec_nrm(tp, &nd);

    if (nd / nf < p.eps)
      break;
  }

  vec_free(tp);
  return 0;
}

int sle_cds_rlx_solve(struct mtx_cds* mp, struct vec* xp, struct vec* fp,
                      struct props p) {
  struct vec* tp = vec_new(xp->n);

  double nd;
  double nf;

  vec_nrm(fp, &nf);

  for (int k = 0; k < p.mk; ++k) {
    step(mp, xp, fp, xp, p.omg);

    mtx_cds_vmlt(mp, xp, tp);

    vec_sub(fp, tp, tp);
    vec_nrm(tp, &nd);

    if (nd / nf < p.eps)
      break;
  }

  vec_free(tp);
  return 0;
}

int sle_cds_brx_solve(struct mtx_cds* mp, struct vec* xp, struct vec* fp,
                      struct props p) {
  return 0;
}
