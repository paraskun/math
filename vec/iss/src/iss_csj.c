#include <iss_csj.h>

#include <string.h>

static int iss_csj_ilu_lsolve(struct mtx_csj* mp, struct vec* xp,
                              struct vec* fp);

static int iss_csj_ilu_usolve(struct mtx_csj* mp, struct vec* xp,
                              struct vec* fp);

int iss_csj_ilu_los_solve(struct mtx_csj* mp, struct vec* xp, struct vec* fp,
                          struct iss_pps pps, void (*f)(int, double)) {
  struct mtx_csj* ilu = mtx_csj_new(mp->n, mp->le, mp->ue);

  struct vec* r = vec_new(mp->n);
  struct vec* z = vec_new(mp->n);
  struct vec* p = vec_new(mp->n);

  double rr;
  double pr;
  double pp;

  double a;
  double b;

  struct vec* t1 = vec_new(mp->n);
  struct vec* t2 = vec_new(mp->n);

  mtx_csj_ilu(mp, ilu);

  // r0 = L^-1 * (f - Ax{0})
  mtx_csj_vmlt(mp, xp, r);
  vec_sub(fp, r, r);
  iss_csj_ilu_lsolve(ilu, r, r);

  // z0 = U^-1 * r{0}
  iss_csj_ilu_usolve(ilu, z, r);

  // p0 = L^-1 * A * z{0}
  mtx_csj_vmlt(mp, z, p);
  iss_csj_ilu_lsolve(ilu, p, p);

  // rr = (r{0}, r{0})
  vec_mlt(r, r, &rr);

  for (int k = 0; k < pps.mk && rr >= pps.eps; ++k) {
    // pr{k-1} = (p{k-1}, r{k-1})
    vec_mlt(p, r, &pr);

    // pp{k-1} = (p{k-1}, p{k-1})
    vec_mlt(p, p, &pp);

    // a{k} = pr{k-1} / pp{k-1}
    a = pr / pp;

    // rr{k} = rr{k-1} - a{k}^2 * pp{k-1}
    rr = rr - a * a * pp;

    // x{k} = x{k-1} + a{k} * z{k-1}
    vec_cmb(xp, z, xp, a);

    // r{k} = r{k-1} - a{k} * p{k-1}
    vec_cmb(r, p, r, -a);

    // t1 = L^-1 * A * U^-1 * r{k}
    iss_csj_ilu_usolve(ilu, t1, r);
    mtx_csj_vmlt(mp, t1, t1);
    iss_csj_ilu_lsolve(ilu, t1, t1);

    // pr = (p{k-1}, t1)
    vec_mlt(p, t1, &pr);

    // b{k} = -pr / pp{k-1}
    b = -pr / pp;

    // t2 = U^-1 * r{k}
    iss_csj_ilu_usolve(ilu, r, t2);

    // z{k} = t2 + b{k} * z{k-1}
    vec_cmb(t2, z, z, b);

    // p{k} = t1 + b{k} * p{k-1}
    vec_cmb(t1, p, p, b);

    f(k, rr);
  }

  vec_free(r);
  vec_free(z);
  vec_free(p);

  vec_free(t1);
  vec_free(t2);

  mtx_csj_free(ilu);

  return 0;
}

static int iss_csj_ilu_lsolve(struct mtx_csj* mp, struct vec* xp,
                              struct vec* fp) {
  int n = mp->n;

  int* mil = mp->il;
  int* mjl = mp->jl;

  double* mdr = mp->dr;
  double* mlr = mp->lr;

  double* xv = xp->vp;
  double* fv = fp->vp;

  for (int i = 0; i < n; ++i) {
    int lr0 = mil[i];
    int lr1 = mil[i + 1];

    xv[i] = fv[i];

    for (int lr = lr0; lr < lr1; ++lr) {
      int lj = mjl[lr];

      xv[i] -= mlr[lr] * xv[lj];
    }

    xv[i] /= mdr[i];
  }

  return 0;
}

static int iss_csj_ilu_usolve(struct mtx_csj* mp, struct vec* xp,
                              struct vec* fp) {
  int n = mp->n;

  int* mju = mp->ju;
  int* miu = mp->iu;

  double* mdr = mp->dr;
  double* mur = mp->ur;

  double* xv = xp->vp;
  double* fv = fp->vp;

  memcpy(fv, xv, sizeof(double) * n);

  for (int j = n - 1; j > -1; --j) {
    xv[j] /= mdr[j];

    int ur0 = mju[j];
    int ur1 = mju[j + 1];

    for (int ur = ur0; ur < ur1; ++ur) {
      int ui = miu[ur];

      xv[ui] -= mur[ur] * xv[j];
    }
  }

  return 0;
}

int iss_csj_los_solve(struct mtx_csj* mp, struct vec* xp, struct vec* fp,
                      struct iss_pps pps, void (*f)(int, double)) {
  struct vec* r = vec_new(mp->n);
  struct vec* z = vec_new(mp->n);
  struct vec* p = vec_new(mp->n);

  double rr;
  double pr;
  double pp;

  double a;
  double b;

  struct vec* t1 = vec_new(mp->n);

  // r{0} = f - A * x{0}
  mtx_csj_vmlt(mp, xp, r);
  vec_sub(fp, r, r);

  // z{0} = r{0}
  vec_cpy(r, z);

  // p{0} = A * z{0}
  mtx_csj_vmlt(mp, z, p);

  // rr = (r{0}, r{0})
  vec_mlt(r, r, &rr);

  for (int k = 0; k < pps.mk && rr >= pps.eps; ++k) {
    // pr{k-1} = (p{k-1}, r{k-1})
    vec_mlt(p, r, &pr);

    // pp{k-1} = (p{k-1}, p{k-1})
    vec_mlt(p, p, &pp);

    // a{k} = pr{k-1} / pp{k-1}
    a = pr / pp;

    // rr{k} = rr{k-1} - a{k}^2 * pp{k-1}
    rr = rr - a * a * pp;

    // x{k} = x{k-1} + a{k} * z{k-1}
    vec_cmb(xp, z, xp, a);

    // r{k} = r{k-1} - a{k} * p{k-1}
    vec_cmb(r, p, r, -a);

    // t1 = A * r{k}
    mtx_csj_vmlt(mp, r, t1);

    // pr = (p{k-1}, t1)
    vec_mlt(p, t1, &pr);

    // b{k} = -pr / pp{k-1}
    b = -pr / pp;

    // z{k} = r{k} + b{k} * z{k-1}
    vec_cmb(r, z, z, b);

    // p{k} = t1 + b{k} * p{k-1}
    vec_cmb(t1, p, p, b);

    f(k, rr);
  }

  vec_free(r);
  vec_free(z);
  vec_free(p);

  vec_free(t1);

  return 0;
}
