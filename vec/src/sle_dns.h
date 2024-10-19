#ifndef DSLE_H
#define DSLE_H

#include <mtx_dns.h>
#include <vec.h>

/**
 * @brief Solve the system of linear equations using Gauss elimination.
 *
 * <p> Ay = b
 *
 * @param mp - coefficients;
 * @param yp - unknowns;
 * @param bp - constant terms;
 */
void dsle_gauss(struct mtx_dns* mp, struct vec* yp, struct vec* bp);

#endif  // DSLE_H
