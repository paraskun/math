#include <vec/iss_csj.h>

#include "vec/mtx_csj.h"
#include "vec/vec.h"

int iss_csj_bcg_slv(struct mtx_csj* mp, struct vec* xp, struct vec* fp,
                    struct iss_pps* pps, struct iss_res* res, fun_iss_cbk cbk) {
  int n = mp->pps.n;

  struct vec* rp = vec_new(n);
  struct vec* zp = vec_new(n);
  struct vec* pp = vec_new(n);
  struct vec* sp = vec_new(n);
  struct vec* hp = vec_new(n);

  double af = 0;
  double bt = 0;
  double om = 0;

  double ro = 0;
  double nn = 0;

  mtx_csj_vmlt(mp, xp, rp);
  vec_cmb(fp, rp, rp, -1);

  vec_cpy(rp, zp);
  vec_cpy(rp, pp);

  vec_mlt(rp, zp, &ro);

  for (int k = 1; k <= pps->mk; ++k) {
    mtx_csj_vmlt(mp, pp, hp);

    vec_mlt(hp, zp, &bt);

    af = ro / bt;

    vec_cmb(rp, hp, sp, -af);

    mtx_csj_vmlt(mp, sp, rp);
    vec_mlt(rp, sp, &om);
    vec_mlt(rp, rp, &bt);

    om = om / bt;

    vec_cmb(xp, pp, xp, af);
    vec_cmb(xp, sp, xp, om);
    vec_cmb(sp, rp, rp, -om);
    vec_nrm(rp, &nn);

    res->k = k;
    res->res = nn;

    if (cbk)
      cbk(res);

    if (nn < pps->eps)
      break;

    vec_mlt(rp, zp, &bt);

    bt = (bt / ro) * (af / om);
    ro = bt;

    vec_cmb(pp, hp, pp, -om);
    vec_cmb(rp, pp, pp, bt);
  }

  vec_cls(rp);
  vec_cls(zp);
  vec_cls(pp);
  vec_cls(sp);
  vec_cls(hp);

  return 0;
}

//static int iss_csj_ilu_lslv(struct mtx_csj* mp, struct vec* xp,
//                            struct vec* fp) {
//  int n = mp->pps.n;
//
//  int* mia = mp->ia;
//  int* mja = mp->ja;
//
//  double* mdr = mp->dr;
//  double* mlr = mp->lr;
//
//  double* xv = xp->vp;
//  double* fv = fp->vp;
//
//  for (int i = 0; i < n; ++i) {
//    int lr0 = mia[i];
//    int lr1 = mia[i + 1];
//
//    double s = 0;
//
//    for (int lr = lr0; lr < lr1; ++lr) {
//      int j = mja[lr];
//
//      s += mlr[lr] * xv[j];
//    }
//
//    xv[i] = (fv[i] - s) / mdr[i];
//  }
//
//  return 0;
//}
//
//static int iss_csj_ilu_uslv(struct mtx_csj* mp, struct vec* xp,
//                            struct vec* fp) {
//  int n = mp->pps.n;
//
//  int* mia = mp->ia;
//  int* mja = mp->ja;
//
//  double* mdr = mp->dr;
//  double* mur = mp->ur;
//
//  double* xv = xp->vp;
//  double* fv = fp->vp;
//
//  memcpy(xv, fv, sizeof(double) * n);
//
//  for (int j = n - 1; j > -1; --j) {
//    xv[j] /= mdr[j];
//
//    int ur0 = mia[j];
//    int ur1 = mia[j + 1];
//
//    double x = xv[j];
//
//    for (int ur = ur0; ur < ur1; ++ur) {
//      int ui = mja[ur];
//
//      xv[ui] -= mur[ur] * x;
//    }
//  }
//
//  return 0;
//}

int iss_csj_bcg_ilu_slv(struct mtx_csj* mp, struct vec* xp, struct vec* fp,
                        struct iss_pps* pps, struct iss_res* res,
                        fun_iss_cbk cbk) {
  int n = mp->pps.n;

  struct mtx_csj* ilu = mtx_csj_new(mp->pps.n, mp->pps.ne);

  struct vec* rp = vec_new(n);
  struct vec* zp = vec_new(n);
  struct vec* pp = vec_new(n);
  struct vec* sp = vec_new(n);
  struct vec* hp = vec_new(n);

  double af = 0;
  double bt = 0;
  double om = 0;

  double ro = 0;
  double nn = 0;

  mtx_csj_ilu(mp, ilu);

  mtx_csj_vmlt(mp, xp, rp);
  vec_cmb(fp, rp, rp, -1);

  vec_cpy(rp, zp);
  vec_cpy(rp, pp);

  vec_mlt(rp, zp, &ro);

  for (int k = 1; k <= pps->mk; ++k) {
    mtx_csj_vmlt(mp, pp, hp);

    vec_mlt(hp, zp, &bt);

    af = ro / bt;

    vec_cmb(rp, hp, sp, -af);

    mtx_csj_vmlt(mp, sp, rp);
    vec_mlt(rp, sp, &om);
    vec_mlt(rp, rp, &bt);

    om = om / bt;

    vec_cmb(xp, pp, xp, af);
    vec_cmb(xp, sp, xp, om);
    vec_cmb(sp, rp, rp, -om);
    vec_nrm(rp, &nn);

    res->k = k;
    res->res = nn;

    if (cbk)
      cbk(res);

    if (nn < pps->eps)
      break;

    vec_mlt(rp, zp, &bt);

    bt = (bt / ro) * (af / om);
    ro = bt;

    vec_cmb(pp, hp, pp, -om);
    vec_cmb(rp, pp, pp, bt);
  }

  vec_cls(rp);
  vec_cls(zp);
  vec_cls(pp);
  vec_cls(sp);
  vec_cls(hp);

  return 0;
}

//static int iss_csj_dgl_slv(struct mtx_csj* mp, struct vec* xp, struct vec* fp) {
//  int n = mp->pps.n;
//
//  double* dr = mp->dr;
//  double* xv = xp->vp;
//  double* fv = fp->vp;
//
//  for (int i = 0; i < n; ++i)
//    xv[i] = fv[i] / dr[i];
//
//  return 0;
//}

int iss_csj_pkt_cls(struct iss_csj_pkt* pkt) {
  iss_pkt_cls(&pkt->pkt);
  mtx_csj_pkt_cls(&pkt->mtx);

  return 0;
}
