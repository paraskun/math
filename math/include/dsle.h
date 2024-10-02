#ifndef DSLE_H
#define DSLE_H

#include <dmtx.h>
#include <vec.h>

void dsle_gauss(struct dmtx* mp, struct vec* yp, struct vec* bp);

#endif  // DSLE_H
