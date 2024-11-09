#ifndef ISS_CMN_H
#define ISS_CMN_H

#include <stdio.h>

struct iss_pps {
  double eps;
  int mk;
};

struct iss_fio {
  FILE* pps;
  FILE* x;
  FILE* f;
};

int iss_pps_fget(struct iss_fio* f, struct iss_pps* pps);

void iss_fio_close(struct iss_fio* f);

#endif  // ISS_CMN_H
