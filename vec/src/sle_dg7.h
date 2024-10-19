#ifndef SLE_DG7_H
#define SLE_DG7_H

#include <mtx_dg7.h>
#include <vec.h>

int sle_dg7_jac_dp(struct mtx_dg7* mp, struct vec* xp, struct vec* fp);

int sle_dg7_rx(struct mtx_dg7* mp, struct vec* xp, struct vec* fp);

int sle_dg7_blk_rx(struct mtx_dg7* mp, struct vec* xp, struct vec* fp);

#endif  // SLE_DG7_H
