#ifndef ISS_CDS_H
#define ISS_CDS_H

#include <vec/iss.h>
#include <vec/mtx_cds.h>
#include <vec/vec.h>

struct iss_jac_pps {
  struct iss_pps pps;

  double omg;
};

typedef int (*fun_iss_cds_slv)(
    struct mtx_cds* mp,
    struct vec* xp,
    struct vec* fp,
    struct iss_jac_pps* pps,
    struct iss_res* res,
    fun_iss_cbk cbk);

int iss_cds_jac_slv(
    struct mtx_cds* mp, 
    struct vec* xp, 
    struct vec* fp,
    struct iss_jac_pps* pps, 
    struct iss_res* res,
    fun_iss_cbk cbk);

int iss_cds_rlx_slv(
    struct mtx_cds* mp, 
    struct vec* xp, 
    struct vec* fp,
    struct iss_jac_pps* pps, 
    struct iss_res* res,
    fun_iss_cbk cbk);

#endif  // ISS_CDS_H
