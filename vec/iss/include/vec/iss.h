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

typedef void (*fun_iss_cbk)(struct iss_res*);

int iss_pkt_cls(struct iss_pkt* pkt);

#endif  // ISS_H
