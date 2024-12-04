#include <vec/iss_csj.h>

#include <string.h>

int iss_csj_bcg_slv(
    struct mtx_csj* mp,
    struct vec* xp, 
    struct vec* fp,
    struct iss_pps* pps, 
    struct iss_res* res, 
    fun_iss_cbk cbk) {
  int n = mp->pps.n;

  struct vec* rp = vec_new(n);
  struct vec* zp = vec_new(n);
  struct vec* pp = vec_new(n);
  struct vec* sp = vec_new(n);
  struct vec* hp = vec_new(n);

  double alf = 0;
  double bet = 0;
  double omg = 0;

  double tmp = 0;
  double nrm = 0;

  mtx_csj_vmlt(mp, xp, rp);
  vec_cmb(fp, rp, rp, -1);

  vec_cpy(rp, zp);
  vec_cpy(rp, pp);

  for (int k = 1; k <= pps->mk; ++k) {
    mtx_csj_vmlt(mp, pp, hp);

    vec_mlt(rp, zp, &tmp);
    vec_mlt(hp, zp, &bet);

    alf = tmp / bet;

    vec_cmb(rp, hp, sp, -alf);

    mtx_csj_vmlt(mp, sp, rp);
    vec_mlt(rp, sp, &omg);
    vec_mlt(rp, rp, &bet);

    omg = omg / bet;

    vec_cmb(xp, pp, xp, alf);
    vec_cmb(xp, sp, xp, omg);
    vec_cmb(sp, rp, rp, -omg);
    vec_nrm(rp, &nrm);

    res->k = k;
    res->res = nrm;

    if (cbk)
      cbk(res);

    if (nrm < pps->eps)
      break;

    vec_mlt(rp, zp, &bet);

    bet = (bet / tmp) * (alf / omg);

    vec_cmb(pp, hp, pp, -omg);
    vec_cmb(rp, pp, pp, bet);
  }

  vec_cls(rp);
  vec_cls(zp);
  vec_cls(pp);
  vec_cls(sp);
  vec_cls(hp);

  return 0;
}

static int iss_csj_con_lslv(
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

static int iss_csj_con_uslv(
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

  if (xp != fp)
    memcpy(xv, fv, sizeof(double) * n);

  for (int j = n - 1; j > -1; --j) {
    xv[j] = xv[j] / mdr[j];

    double x = xv[j];

    int ur0 = mia[j];
    int ur1 = mia[j + 1];

    for (int ur = ur0; ur < ur1; ++ur) {
      int ui = mja[ur];

      xv[ui] -= mur[ur] * x;
    }
  }

  return 0;
}

int iss_csj_bcg_con_slv(
    struct mtx_csj* mp, 
    struct mtx_csj* cp,
    struct vec* xp, 
    struct vec* fp,
    struct iss_pps* pps, 
    struct iss_res* res,
    fun_iss_cbk cbk) {
  int n = mp->pps.n;

  struct vec* rp = vec_new(n);
  struct vec* zp = vec_new(n);
  struct vec* pp = vec_new(n);
  struct vec* sp = vec_new(n);
  struct vec* hp = vec_new(n);

  struct vec* pt = vec_new(n);
  struct vec* st = vec_new(n);

  double alf = 0;
  double bet = 0;
  double omg = 0;

  double tmp = 0;
  double nrm = 0;

  mtx_csj_vmlt(mp, xp, rp);
  vec_cmb(fp, rp, rp, -1);

  vec_cpy(rp, zp);
  vec_cpy(rp, pp);

  for (int k = 1; k <= pps->mk; ++k) {
    iss_csj_con_lslv(cp, pt, pp);
    iss_csj_con_uslv(cp, pt, pt);

    mtx_csj_vmlt(mp, pt, hp);

    vec_mlt(rp, zp, &tmp);
    vec_mlt(hp, zp, &bet);

    alf = tmp / bet;

    vec_cmb(rp, hp, sp, -alf);

    iss_csj_con_lslv(cp, st, sp);
    iss_csj_con_uslv(cp, st, st);

    mtx_csj_vmlt(mp, sp, rp);
    vec_mlt(rp, sp, &omg);
    vec_mlt(rp, rp, &bet);

    omg = omg / bet;

    vec_cmb(xp, pt, xp, alf);
    vec_cmb(xp, st, xp, omg);
    vec_cmb(sp, rp, rp, -omg);
    vec_nrm(rp, &nrm);

    res->k = k;
    res->res = nrm;

    if (cbk)
      cbk(res);

    if (nrm < pps->eps)
      break;

    vec_mlt(rp, zp, &bet);

    bet = (bet / tmp) * (alf / omg);

    vec_cmb(pp, hp, pp, -omg);
    vec_cmb(rp, pp, pp, bet);
  }

  vec_cls(rp);
  vec_cls(zp);
  vec_cls(pp);
  vec_cls(sp);
  vec_cls(hp);

  return 0;
}

int iss_csj_pkt_cls(struct iss_csj_pkt* pkt) {
  iss_pkt_cls(&pkt->pkt);
  mtx_csj_pkt_cls(&pkt->mtx);

  return 0;
}
