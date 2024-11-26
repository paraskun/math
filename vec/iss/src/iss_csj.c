#include <dss_csj.h>
#include <iss_csj.h>

#include <math.h>

int iss_csj_los_slv(
    struct mtx_csj* mp, 
    struct vec* xp, 
    struct vec* fp,
    struct iss_pps* pps, 
    struct iss_res* res,
    fun_iss_cbk cbk) {
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

  mtx_csj_vmlt(mp, xp, r);
  vec_cmb(fp, r, r, -1);

  vec_cpy(r, z);

  mtx_csj_vmlt(mp, z, p);

  vec_mlt(r, r, &rr);

  for (int k = 0; k < mk && rr >= eps; ++k) {
    vec_mlt(p, r, &pr);
    vec_mlt(p, p, &pp);

    a = pr / pp;
    rr = rr - a * a * pp;

    vec_cmb(xp, z, xp, a);
    vec_cmb(r, p, r, -a);
    mtx_csj_vmlt(mp, r, t1);
    vec_mlt(p, t1, &pr);

    b = -pr / pp;

    vec_cmb(r, z, z, b);
    vec_cmb(t1, p, p, b);
    
    res->res = rr;
    res->k = k + 1;

    if (cbk)
      cbk(res);
  }

  vec_cls(r);
  vec_cls(z);
  vec_cls(p);
  vec_cls(t1);

  return 0;
}

int iss_csj_ilu_los_slv(
    struct mtx_csj* mp, 
    struct vec* xp, 
    struct vec* fp,
    struct iss_pps* pps, 
    struct iss_res* res,
    fun_iss_cbk cbk) {
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

  int mk = pps->mk;
  double eps = pps->eps;

  mtx_csj_ilu(mp, ilu);
  mtx_csj_vmlt(mp, xp, r);
  vec_cmb(fp, r, r, -1);
  iss_csj_ilu_lslv(ilu, r, r);
  iss_csj_ilu_uslv(ilu, z, r);
  mtx_csj_vmlt(mp, z, p);
  iss_csj_ilu_lslv(ilu, p, p);

  vec_mlt(r, r, &rr);

  for (int k = 0; k < mk && rr >= eps; ++k) {
    vec_mlt(p, r, &pr);
    vec_mlt(p, p, &pp);

    a = pr / pp;
    rr = rr - a * a * pp;

    vec_cmb(xp, z, xp, a);
    vec_cmb(r, p, r, -a);
    iss_csj_ilu_uslv(ilu, t1, r);
    mtx_csj_vmlt(mp, t1, t1);
    iss_csj_ilu_lslv(ilu, t1, t1);
    vec_mlt(p, t1, &pr);

    b = -pr / pp;

    iss_csj_ilu_uslv(ilu, t2, r);
    vec_cmb(t2, z, z, b);
    vec_cmb(t1, p, p, b);

    res->res = rr;
    res->k = k + 1;

    if (cbk)
      cbk(res);
  }

  mtx_csj_cls(ilu);

  vec_cls(r);
  vec_cls(z);
  vec_cls(p);
  vec_cls(t1);
  vec_cls(t2);

  return 0;
}

int iss_csj_dgl_los_slv(
    struct mtx_csj* mp, 
    struct vec* xp, 
    struct vec* fp,
    struct iss_pps* pps, 
    struct iss_res* res,
    fun_iss_cbk cbk) {
  int n = mp->pps.n;

  struct mtx_csj_pps dgl_pps = {n, 0};
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
  iss_csj_dgl_slv(dgl, r, r);
  iss_csj_dgl_slv(dgl, z, r);
  mtx_csj_vmlt(mp, z, p);
  iss_csj_dgl_slv(dgl, p, p);
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
    iss_csj_dgl_slv(dgl, t1, r);
    mtx_csj_vmlt(mp, t1, t1);
    iss_csj_dgl_slv(dgl, t1, t1);
    vec_mlt(p, t1, &pr);

    b = -pr / pp;

    iss_csj_dgl_slv(dgl, r, t2);
    vec_cmb(t2, z, z, b);
    vec_cmb(t1, p, p, b);

    res->res = rr;
    res->k = k + 1;

    if (cbk)
      cbk(res);
  }

  mtx_csj_cls(dgl);

  vec_cls(r);
  vec_cls(z);
  vec_cls(p);
  vec_cls(t1);
  vec_cls(t2);

  return 0;
}


int iss_csj_pkt_cls(struct iss_csj_pkt* pkt) {
  iss_pkt_cls(&pkt->pkt);
  mtx_csj_pkt_cls(&pkt->mtx);

  return 0;
}
