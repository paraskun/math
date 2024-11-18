#ifndef DSS_SKY_H
#define DSS_SKY_H

#include <mtx_sky.h>
#include <vec.h>

void dss_sky_ldu_solve(
    struct mtx_sky* mp, 
    struct vec* xp, 
    struct vec* fp);

#endif  // DSS_SKY_H
