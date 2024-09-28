#ifndef SLE_H
#define SLE_H

#include <mtx.h>
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
void sle_gauss(struct mtx* mp, struct vec* yp, struct vec* bp);

/**
 * @brief Solve the system of linear equations using LDU decomposition.
 *
 * <p> Ay = b
 *
 * @param mp - coefficients;
 * @param yp - unknowns;
 * @param bp - constant terms;
 */
void sle_ldu(struct mtx* mp, struct vec* yp, struct vec* bp);

#endif  // SLE_H
