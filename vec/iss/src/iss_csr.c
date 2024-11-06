#include <iss_csr.h>

#include <stdlib.h>

static int iss_csr_ilu_lsolve(struct mtx_csr* mp, struct vec* xp,
                              struct vec* fp);

static int iss_csr_ilu_usolve(struct mtx_csr* mp, struct vec* xp,
                              struct vec* fp);

int iss_csr_ilu_los_solve(struct mtx_csr* mp, struct vec* xp, struct vec* fp,
                          struct iss_pps pps, void (*f)(int, double)) {
  f(0, 0.0);

  iss_csr_ilu_lsolve(mp, xp, fp);
  iss_csr_ilu_usolve(mp, xp, fp);

  free(mp);
  free(xp);
  free(fp);

  return pps.ms;
}

static int iss_csr_ilu_lsolve(struct mtx_csr* mp, struct vec* xp,
                              struct vec* fp) {
  free(mp);
  free(xp);
  free(fp);

  return 0;
}

static int iss_csr_ilu_usolve(struct mtx_csr* mp, struct vec* xp,
                              struct vec* fp) {
  free(mp);
  free(xp);
  free(fp);

  return 0;
}
