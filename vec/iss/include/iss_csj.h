#ifndef ISS_CSJ_H
#define ISS_CSJ_H

#include <iss.h>
#include <mtx_csj.h>
#include <vec.h>

typedef int (*fun_iss_csj_slv)(
    struct mtx_csj* mp, 
    struct vec* xp, 
    struct vec* fp,
    struct iss_pps* pps,
    struct iss_res* res,
    fun_iss_cbk cbk);

int iss_csj_los_slv(
    struct mtx_csj* mp, 
    struct vec* xp, 
    struct vec* fp,
    struct iss_pps* pps, 
    struct iss_res* res,
    fun_iss_cbk cbk);

int iss_csj_ilu_los_slv(
    struct mtx_csj* mp, 
    struct vec* xp, 
    struct vec* fp,
    struct iss_pps* pps,
    struct iss_res* res,
    fun_iss_cbk cbk);

int iss_csj_dgl_los_slv(
    struct mtx_csj* mp, 
    struct vec* xp, 
    struct vec* fp,
    struct iss_pps* pps, 
    struct iss_res* res,
    fun_iss_cbk cbk);

#endif  // ISS_CSJ_H
