#ifndef SLE_H
#define SLE_H

#include <mtx/mtx.h>
#include <vec/vec.h>

/**
 * @brief Solve the system of linear equations using Gauss elimination.
 *
 * <p> Ax = b
 *
 * @param a - coefficients;
 * @param x - unknowns;
 * @param b - constant terms;
 */
void sle_gauss(struct mtx* a, struct vec* x, struct vec* b);

/**
 * @brief Solve the system of linear equations using LDU decomposition.
 *
 * <p> Ax = b
 *
 * @param a - coefficients;
 * @param x - unknowns;
 * @param b - constant terms;
 */
void sle_ldu(struct mtx* a, struct vec* x, struct vec* b);

#endif  // SLE_H
