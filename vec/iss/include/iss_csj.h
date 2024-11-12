#ifndef ISS_CSJ_H
#define ISS_CSJ_H

#include <iss.h>
#include <mtx_csj.h>
#include <vec.h>

int iss_csj_los_slv(struct mtx_csj* mp, struct vec* xp, struct vec* fp,
                    struct iss_pps* pps, struct iss_res* res,
                    void (*f)(struct iss_res*));

int iss_csj_ilu_los_slv(struct mtx_csj* mp, struct vec* xp, struct vec* fp,
                        struct iss_pps* pps, struct iss_res* res,
                        void (*f)(struct iss_res*));

int iss_csj_dgl_los_slv(struct mtx_csj* mp, struct vec* xp, struct vec* fp,
                        struct iss_pps* pps, struct iss_res* res,
                        void (*f)(struct iss_res*));

#endif  // ISS_CSJ_H
