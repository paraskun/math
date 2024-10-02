#ifndef SSLE_H
#define SSLE_H

#include <smtx.h>
#include <vec.h>

/**
 * @brief Solve the system of linear equations using LDU decomposition.
 *
 * <p> Ay = b
 *
 * @param mp - coefficients;
 * @param yp - unknowns;
 * @param bp - constant terms;
 */
void ssle_ldu(struct smtx* mp, struct vec* yp, struct vec* bp);

#endif  // SSLE_H
