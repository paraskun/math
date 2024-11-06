#ifndef ISS_CMN_H
#define ISS_CMN_H

#include <stdio.h>

struct iss_pps {
  double eps;
  int mk;
};

int iss_pps_fget(FILE* f, struct iss_pps* pps);

#endif  // ISS_CMN_H
