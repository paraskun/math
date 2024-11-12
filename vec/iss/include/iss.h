#ifndef ISS_H
#define ISS_H

#include <stdio.h>

struct iss_pps {
  double eps;
  int mk;
};

struct iss_res {
  double res;
  int k;
};

struct iss_pkt {
  FILE* pps;
  FILE* x;
  FILE* f;
};

int iss_pps_fget(struct iss_pkt* pkt, struct iss_pps* pps);
int iss_pkt_close(struct iss_pkt* pkt);

#endif  // ISS_H
