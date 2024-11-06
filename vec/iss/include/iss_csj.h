#ifndef ISS_CSR_H
#define ISS_CSR_H

#include <iss_cmn.h>
#include <mtx_csj.h>
#include <vec.h>

int iss_csj_los_solve(struct mtx_csj* mp, struct vec* xp, struct vec* fp,
                      struct iss_pps, void (*f)(int, double));

int iss_csj_ilu_los_solve(struct mtx_csj* mp, struct vec* xp, struct vec* fp,
                          struct iss_pps pps, void (*f)(int, double));

#endif  // ISS_CSR_H
