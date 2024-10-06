#ifndef PMTX_H
#define PMTX_H

#include <stdio.h>
#include <vec.h>

struct pmtx {
  real* v;

  int n;
  int rf;
  int rc;
};

struct pmtx* pmtx_new(int n, int rf, int rc);

int pmtx_fget(FILE* f, struct pmtx* mp);
int pmtx_vmlt(struct pmtx* mp, struct vec* xp, struct vec* bp);

int pmtx_free(struct pmtx* mp);

#endif  // PMTX_H
