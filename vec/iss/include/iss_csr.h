#ifndef ISS_CSR_H
#define ISS_CSR_H

#include <iss_cmn.h>
#include <mtx_csr.h>
#include <vec.h>

int iss_csr_los_solve(struct mtx_csr*, struct vec*, struct vec*, struct iss_pps,
                      void (*f)(int, double));

#endif  // ISS_CSR_H
