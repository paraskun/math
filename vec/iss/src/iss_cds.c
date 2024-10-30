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
                      struct pps pps, struct rep* rep) {
  struct vec* tp = vec_new(xp->n);

  double nf;

  vec_nrm(fp, &nf);

  double nd = nf;
  double r = 1;

  for (int s = 0; s < pps.ms && r >= pps.eps; ++s) {
    step(mp, xp, fp, tp, pps.omg);
    swap(xp, tp);

    mtx_cds_vmlt(mp, xp, tp);
    vec_sub(fp, tp, tp);
    vec_nrm(tp, &nd);

    r = nd / nf;

    rep->sc = s;
    rep->res = r;

    if (pps.dbg)
      printf("\rIteration: %d; Residual: %.7e", s + 1, r);
  }

  if (pps.dbg)
    putchar('\n');

  vec_free(tp);
  return 0;
}

int iss_cds_rlx_solve(struct mtx_cds* mp, struct vec* xp, struct vec* fp,
                      struct pps pps, struct rep* rep) {
  struct vec* tp = vec_new(xp->n);

  double nf;

  vec_nrm(fp, &nf);

  double nd = nf;
  double r = 1;

  for (int s = 0; s < pps.ms && r >= pps.eps; ++s) {
    step(mp, xp, fp, xp, pps.omg);

    mtx_cds_vmlt(mp, xp, tp);
    vec_sub(fp, tp, tp);
    vec_nrm(tp, &nd);

    r = nd / nf;

    rep->sc = s;
    rep->res = r;
  }

  vec_free(tp);
  return 0;
}

static int brx_lu(struct mtx_cds* mp, int bs, int bc);
static int brx_lu_solve(struct mtx_cds* mp, int bs, int bn, struct vec* xp,
                        struct vec* rp);

int iss_cds_brx_solve(struct mtx_cds* mp, struct vec* xp, struct vec* fp,
                      struct pps p) {
  struct vec* rp = vec_new(xp->n);

  int n = mp->n;
  int c = mp->c;
  int ms = p.ms;
  int bs = p.bs;
  int bc = n / bs;

  int* la = mp->la;
  double** ad = mp->ad;
  double* xvp = xp->vp;
  double* fvp = fp->vp;
  double* rvp = rp->vp;

  for (int b = 0; b < bc; ++b)
    brx_lu(mp, bs, bc);

  double nd;
  double nf;

  vec_nrm(fp, &nf);

  for (int s = 0; s < ms; ++s) {
    for (int b = 0; b < bc; ++b) {
      int i0 = b * bs;
      int i1 = i0 + bs;

      for (int i = i0, ir = 0; i < i1; ++i, ++ir)
        rvp[ir] = fvp[i];

      for (int i = i0, ir = 0; i < i1; ++i, ++ir) {
        for (int k = 0; k < c; ++k) {
          int j = la[k] + i;

          if (j < 0 || j >= n)
            continue;

          rvp[ir] -= ad[i][k] * xvp[j];
        }

        rvp[ir] *= p.omg;
      }

      brx_lu_solve(mp, bs, b, xp, rp);
    }

    mtx_cds_vmlt(mp, xp, rp);

    vec_sub(fp, rp, rp);
    vec_nrm(rp, &nd);

    if (nd / nf < p.eps)
      break;
  }

  vec_free(rp);

  return 0;
}

static int brx_lu(struct mtx_cds* mp, int bs, int bc) {
  return mp->n + bs + bc;
}

static int brx_lu_solve(struct mtx_cds* mp, int bs, int bn, struct vec* xp,
                        struct vec* rp) {
  return mp->n + bs + bn + xp->n + rp->n;
}
