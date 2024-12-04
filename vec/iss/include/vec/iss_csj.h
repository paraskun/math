#ifndef ISS_CSJ_H
#define ISS_CSJ_H

#include <vec//iss.h>
#include <vec/mtx_csj.h>
#include <vec/vec.h>

struct iss_csj_pkt {
  struct iss_pkt pkt;
  struct mtx_csj_pkt mtx;
};

int iss_csj_bcg_slv(
    struct mtx_csj* mp, 
    struct vec* xp, 
    struct vec* fp,
    struct iss_pps* pps, 
    struct iss_res* res,
    fun_iss_cbk cbk);

int iss_csj_bcg_con_slv(
    struct mtx_csj* mp, 
    struct mtx_csj* cp,
    struct vec* xp, 
    struct vec* fp,
    struct iss_pps* pps, 
    struct iss_res* res,
    fun_iss_cbk cbk);

int iss_csj_pkt_cls(struct iss_csj_pkt* pkt);

#endif  // ISS_CSJ_H
