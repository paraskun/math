#ifndef PMTX_H
#define PMTX_H

#include <vec.h>

#include <stdio.h>

struct pmtx {
  real* v;

  int n;  // Matrix dimentsions
  int s;  // Initial row
  int c;  // Number of rows
};

struct pmtx* pmtx_new(int n, int s, int c);

int pmtx_fget(FILE* f, struct pmtx* mp);
int pmtx_fput(FILE* f, struct pmtx* mp);
int pmtx_seq(struct pmtx* mp, int s);

int pmtx_vmlt(struct pmtx* mp, struct vec* xp, struct vec* bp);

int pmtx_free(struct pmtx* mp);

#endif  // PMTX_H
