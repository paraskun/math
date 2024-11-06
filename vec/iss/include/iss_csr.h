#ifndef ISS_CSR_H
#define ISS_CSR_H

#include <iss_cmn.h>
#include <mtx_csr.h>
#include <vec.h>

int iss_csr_ilu_los_solve(struct mtx_csr* mp, struct vec* xp, struct vec* fp,
                          struct iss_pps pps, void (*f)(int, double));

#endif  // ISS_CSR_H
