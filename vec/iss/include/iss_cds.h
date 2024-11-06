#ifndef ISS_CDS_H
#define ISS_CDS_H

#include <iss_cmn.h>
#include <mtx_cds.h>
#include <vec.h>

struct iss_jac_pps {
  struct iss_pps pps;

  double omg;
};

int iss_cds_jac_solve(struct mtx_cds* mp, struct vec* xp, struct vec* fp,
                      struct iss_jac_pps pps, void (*f)(int, double));

int iss_cds_rlx_solve(struct mtx_cds* mp, struct vec* xp, struct vec* fp,
                      struct iss_jac_pps pps, void (*f)(int, double));

#endif  // ISS_CDS_H
