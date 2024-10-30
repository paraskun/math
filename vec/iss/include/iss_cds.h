#ifndef ISS_CDS_H
#define ISS_CDS_H

#include <mtx_cds.h>
#include <vec.h>

struct pps {
  double omg;
  double eps;

  int ms;
  int bs;

  int dbg;
};

struct rep {
  int sc;
  double res;
};

int iss_cds_jac_solve(struct mtx_cds*, struct vec*, struct vec*, struct pps,
                      struct rep*);
int iss_cds_rlx_solve(struct mtx_cds*, struct vec*, struct vec*, struct pps,
                      struct rep*);
int iss_cds_brx_solve(struct mtx_cds*, struct vec*, struct vec*, struct pps);

#endif  // ISS_CDS_H
