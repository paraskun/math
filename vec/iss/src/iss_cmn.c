#include <iss_cmn.h>

int iss_pps_fget(FILE* f, struct iss_pps* pps) {
  fscanf(f, "%lf", &pps->eps);
  fscanf(f, "%d", &pps->ms);

  return 0;
}
