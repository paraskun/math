#include <iss_csj.h>

#include <math.h>
#include <string.h>

int iss_csj_los_solve(struct mtx_csj* mp, struct vec* xp, struct vec* fp,
                      struct iss_pps* pps, void (*f)(int, double)) {
  int n = mp->pps.n;

  struct vec* r = vec_new(n);
  struct vec* z = vec_new(n);
  struct vec* p = vec_new(n);

  double rr;
  double pr;
  double pp;

  double a;
  double b;

  struct vec* t1 = vec_new(n);

  int mk = pps->mk;
  double eps = pps->eps;

  // r{0} = f - A * x{0}
  mtx_csj_vmlt(mp, xp, r);
  vec_cmb(fp, r, r, -1);

  // z{0} = r{0}
  vec_cpy(r, z);

  // p{0} = A * z{0}
  mtx_csj_vmlt(mp, z, p);

  // rr{0} = r{0} * r{0}
  vec_mlt(r, r, &rr);

  for (int k = 0; k < mk && fabs(rr) >= eps; ++k) {
    // pr{k-1} = p{k-1} * r{k-1}
    vec_mlt(p, r, &pr);

    // pp{k-1} = p{k-1} * p{k-1}
    vec_mlt(p, p, &pp);

    // a{k} == pr{k-1} / pp{k-1}
    a = pr / pp;

    // rr{k} = rr{k-1} - a{k} * a{k} * pp{k-1}
    rr = rr - a * a * pp;

    // x{k} = x{k-1} + a{k} * z{k-1}
    vec_cmb(xp, z, xp, a);

    // r{k} = r{k-1} - a{k} * p{k-1}
    vec_cmb(r, p, r, -a);

    // pr = p{k-1} * A * r{k}
    mtx_csj_vmlt(mp, r, t1);
    vec_mlt(p, t1, &pr);

    // b{k} = -pr / pp{k-1}
    b = -pr / pp;

    // z{k} = r{k} + b{k} * z{k-1}
    vec_cmb(r, z, z, b);

    // p{k} = A * r{k} + b{k} + p{k-1}
    vec_cmb(t1, p, p, b);

    f(k, rr);
  }

  vec_free(r);
  vec_free(z);
  vec_free(p);

  vec_free(t1);

  return 0;
}

int iss_csj_ilu_lsolve(struct mtx_csj* mp, struct vec* xp, struct vec* fp) {
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

int iss_csj_ilu_usolve(struct mtx_csj* mp, struct vec* xp, struct vec* fp) {
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

int iss_csj_ilu_los_solve(struct mtx_csj* mp, struct vec* xp, struct vec* fp,
                          struct iss_pps* pps, void (*f)(int, double)) {
  int n = mp->pps.n;

  struct mtx_csj* ilu = mtx_csj_new(mp->pps);

  struct vec* r = vec_new(n);
  struct vec* z = vec_new(n);
  struct vec* p = vec_new(n);

  double rr;
  double pr;
  double pp;

  double a;
  double b;

  struct vec* t1 = vec_new(n);
  struct vec* t2 = vec_new(n);

  mtx_csj_ilu(mp, ilu);

  putchar('\n');
  mtx_csj_fput_all(stdout, ilu);
  putchar('\n');

  // mtx_csj_fput_all(stdout, ilu);

  // r{0} = L^-1 * (f - A * x{0})
  mtx_csj_vmlt(mp, xp, r);
  vec_cmb(fp, r, r, -1);
  iss_csj_ilu_lsolve(ilu, r, r);

  // z{0} = U^-1 * r{0}
  iss_csj_ilu_usolve(ilu, z, r);

  // p{0} = L^-1 * A * z{0}
  mtx_csj_vmlt(mp, z, p);
  iss_csj_ilu_lsolve(ilu, p, p);

  // rr{0} = (r{0}, r{0})
  vec_mlt(r, r, &rr);

  int mk = pps->mk;
  double eps = pps->eps;

  for (int k = 0; k < mk && fabs(rr) >= eps; ++k) {
    // pr{k-1} = (p{k-1}, r{k-1})
    vec_mlt(p, r, &pr);

    // pp{k-1} = (p{k-1}, p{k-1})
    vec_mlt(p, p, &pp);

    // a{k} = pr{k-1} / pp{k-1}
    a = pr / pp;

    // rr{k} = rr{k-1} * a{k}^2 * pp{k-1}
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

int iss_csj_dgl_solve(struct mtx_csj* mp, struct vec* xp, struct vec* fp) {
  int n = mp->pps.n;

  double* mdr = mp->dr;
  double* xvp = xp->vp;
  double* fvp = fp->vp;

  for (int i = 0; i < n; ++i)
    xvp[i] = fvp[i] / mdr[i];

  return 0;
}

int iss_csj_dgl_los_solve(struct mtx_csj* mp, struct vec* xp, struct vec* fp,
                          struct iss_pps* pps, void (*f)(int, double)) {
  int n = mp->pps.n;

  struct mtx_csj_pps dgl_pps = {
      .n = n,
      .le = 0,
      .ue = 0,
  };

  struct mtx_csj* dgl = mtx_csj_new(dgl_pps);

  struct vec* r = vec_new(n);
  struct vec* z = vec_new(n);
  struct vec* p = vec_new(n);

  double rr;
  double pr;
  double pp;

  double a;
  double b;

  struct vec* t1 = vec_new(n);
  struct vec* t2 = vec_new(n);

  mtx_csj_dgl(mp, dgl);
  mtx_csj_vmlt(mp, xp, r);
  vec_cmb(fp, r, r, -1);
  iss_csj_dgl_solve(dgl, r, r);
  iss_csj_dgl_solve(dgl, z, r);
  mtx_csj_vmlt(mp, z, p);
  iss_csj_dgl_solve(dgl, p, p);
  vec_mlt(r, r, &rr);

  int mk = pps->mk;
  double eps = pps->eps;

  for (int k = 0; k < mk && rr >= eps; ++k) {
    vec_mlt(p, r, &pr);
    vec_mlt(p, p, &pp);

    a = pr / pp;
    rr = rr - a * a * pp;

    vec_cmb(xp, z, xp, a);
    vec_cmb(r, p, r, -a);
    iss_csj_dgl_solve(dgl, t1, r);
    mtx_csj_vmlt(mp, t1, t1);
    iss_csj_dgl_solve(dgl, t1, t1);
    vec_mlt(p, t1, &pr);

    b = -pr / pp;

    iss_csj_dgl_solve(dgl, r, t2);
    vec_cmb(t2, z, z, b);
    vec_cmb(t1, p, p, b);

    f(k, rr);
  }

  vec_free(r);
  vec_free(z);
  vec_free(p);

  vec_free(t1);
  vec_free(t2);

  mtx_csj_free(dgl);

  return 0;
  return 0;
}

void iss_csj_fio_close(struct iss_csj_fio* f) {
  iss_fio_close(&f->f);
  mtx_csj_fio_close(&f->m);
}
