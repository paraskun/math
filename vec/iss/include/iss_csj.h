#ifndef ISS_CSR_H
#define ISS_CSR_H

#include <iss_cmn.h>
#include <mtx_csj.h>
#include <vec.h>

struct iss_csj_fio {
  struct iss_fio f;
  struct mtx_csj_fio m;
};

int iss_csj_los_solve(struct mtx_csj* mp, struct vec* xp, struct vec* fp,
                      struct iss_pps* pps, void (*f)(int, double));

int iss_csj_ilu_lsolve(struct mtx_csj* mp, struct vec* xp, struct vec* fp);
int iss_csj_ilu_usolve(struct mtx_csj* mp, struct vec* xp, struct vec* fp);

int iss_csj_ilu_los_solve(struct mtx_csj* mp, struct vec* xp, struct vec* fp,
                          struct iss_pps* pps, void (*f)(int, double));

int iss_csj_dgl_solve(struct mtx_csj* mp, struct vec* xp, struct vec* fp);

int iss_csj_dgl_los_solve(struct mtx_csj* mp, struct vec* xp, struct vec* fp,
                          struct iss_pps* pps, void (*f)(int, double));

void iss_csj_fio_close(struct iss_csj_fio* f);

#endif  // ISS_CSR_H
