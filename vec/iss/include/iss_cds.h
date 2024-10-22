#ifndef ISS_CDS_H
#define ISS_CDS_H

#include <mtx_cds.h>
#include <vec.h>

struct props {
  double omg;
  double eps;

  int mk;
};

int iss_cds_jac_solve(struct mtx_cds* mp, struct vec* xp, struct vec* fp,
                      struct props p);

int sle_cds_rlx_solve(struct mtx_cds* mp, struct vec* xp, struct vec* fp,
                      struct props p);

int sle_cds_brx_solve(struct mtx_cds* mp, struct vec* xp, struct vec* fp,
                      struct props p);

#endif  // ISS_CDS_H
