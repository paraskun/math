#ifndef DMTX_H

#include <mtx/mtx.h>

#include <stddef.h>

#define ME(m, i, j) (m->v[(i) * m->n + (j)])

/**
 * @class mtx
 * @brief Dense matrix storage.
 */
struct mtx {
  real* v;
  size_t n;
};

/**
 * @brief Allocate memory and return a new matrix with undefined values.
 *
 * @param n - matrix dimensions;
 */
struct mtx* mtx_new(size_t n);

/**
 * @brief Convert given matrix to a matrix with randomly generated values. 
 *
 * @param n - matrix to convert;
 * @param u - values upper-bound;
 */
void mtx_rnd(struct mtx* mp, size_t u);

/**
 * @brief Convert given matrix to a new matrix with sequential values starting from 1.
 *
 * @param n - matrix dimensions;
 */
void mtx_seq(struct mtx* mp);

/**
 * @brief Calculate the matrix-matrix product.
 *
 * @param a - left-hand operand;
 * @param b - right-hand operand;
 * @param c - result;
 */
void mtx_mmlt(struct mtx* ap, struct mtx* bp, struct mtx* cp);

/**
 * @brief Calculate the matrix-vector product.
 *
 * @param a - left-hand operand;
 * @param b - right-hand operand;
 * @param c - result;
 */
void mtx_vmlt(struct mtx* ap, struct vec* bp, struct vec* cp);

/**
 * @brief Calculate the matrix norm.
 *
 * @param a - operand;
 * @param r - result;
 */
void mtx_norm(struct mtx* ap, real* rp);

#endif  // MTX_DNS_H
