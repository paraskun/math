#ifndef DSS_CSJ_H
#define DSS_CSJ_H

#include <mtx_csj.h>

int iss_csj_ilu_lslv(
    struct mtx_csj* mp, 
    struct vec* xp, 
    struct vec* fp);

int iss_csj_ilu_uslv(
    struct mtx_csj* mp, 
    struct vec* xp, 
    struct vec* fp);

int iss_csj_dgl_slv(
    struct mtx_csj* mp, 
    struct vec* xp, 
    struct vec* fp);

#endif  // DSS_CSJ_H
