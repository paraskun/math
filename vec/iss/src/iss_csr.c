#include <iss_csr.h>

#include <stdlib.h>

static int iss_csr_ilu_lsolve(struct mtx_csr* mp, struct vec* xp,
                              struct vec* fp);

static int iss_csr_ilu_usolve(struct mtx_csr* mp, struct vec* xp,
                              struct vec* fp);

int iss_csr_ilu_los_solve(struct mtx_csr* mp, struct vec* xp, struct vec* fp,
                          struct iss_pps pps, void (*f)(int, double)) {
  struct mtx_csr* ilu = mtx_csr_new(mp->n, mp->ne);

  mtx_csr_ilu(mp, ilu);

  struct vec* r = vec_new(mp->n);
  struct vec* z = vec_new(mp->n);
  struct vec* p = vec_new(mp->n);

  struct vec* t1 = vec_new(mp->n);
  struct vec* t2 = vec_new(mp->n);

  mtx_csr_vmlt(mp, xp, r);
  vec_sub(fp, r, r);
  iss_csr_ilu_lsolve(ilu, r, r);

  iss_csr_ilu_usolve(ilu, z, r);

  mtx_csr_vmlt(mp, z, p);
  iss_csr_ilu_lsolve(ilu, p, p);

  double rr;

  vec_mlt(r, r, &rr);

  for (int s = 0; s < pps.ms && rr >= pps.eps; ++s) {
    double pr = 0;
    double pp = 0;

    vec_mlt(p, r, &pr);
    vec_mlt(p, p, &pp);

    double a = pr / pp;

    rr = rr - a * a * pp;

    vec_cmb(xp, z, xp, a);
    vec_cmb(r, p, r, -a);

    iss_csr_ilu_usolve(ilu, t1, r);
    mtx_csr_vmlt(mp, t1, t1);
    iss_csr_ilu_lsolve(ilu, t1, t1);

    vec_mlt(p, t1, &pr);

    double b = pr / pp;

    iss_csr_ilu_usolve(ilu, r, t2);

    vec_cmb(t2, z, z, b);
    vec_cmb(t1, p, p, b);

    f(s, rr);
  }

  vec_free(r);
  vec_free(z);
  vec_free(p);

  vec_free(t1);
  vec_free(t2);

  return 0;
}

static int iss_csr_ilu_lsolve(struct mtx_csr* mp, struct vec* xp,
                              struct vec* fp) {
  int n = mp->n;

  int* ia = mp->ia;
  int* ja = mp->ja;

  double* xv = xp->vp;
  double* fv = fp->vp;
  double* ar = mp->ar;

  for (int i = 0; i < n; ++i) {
    int r0 = ia[i];
    int r1 = ia[i + 1];

    xv[i] = fv[i];

    for (int r = r0; r < r1; ++r) {
      int j = ja[r];

      if (j > i)
        return -1;

      if (j == i) {
        xv[i] /= ar[r];
        break;
      }

      xv[i] -= ar[r] * xv[j];
    }
  }

  return 0;
}

static int iss_csr_ilu_usolve(struct mtx_csr* mp, struct vec* xp,
                              struct vec* fp) {

  return 0;
}

int iss_csr_los_solve(struct mtx_csr* mp, struct vec* xp, struct vec* fp,
                      struct iss_pps, void (*f)(int, double)) {
  return 0;
}
