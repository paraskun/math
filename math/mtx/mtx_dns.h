#ifndef MTX_DNS_H

#include <mtx.h>

#include <stddef.h>

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
 * @brief Allocate memory and return a new matrix with randomly generated values. 
 *
 * @param n - matrix dimensions;
 * @param u - values upper-bound;
 */
struct mtx* mtx_rnd(size_t n, size_t u);

/**
 * @brief Allocate memory and return a new matrix with sequential values started from zero.
 *
 * @param n - matrix dimensions;
 */
struct mtx* mtx_seq(size_t n);

/**
 * @brief Write the values from the given matrix to standard output.
 *
 * <p> Not recommended for large matrices.
 *
 * @param a - matrix to write;
 */
void mtx_cput(struct mtx* a);

/**
 * @brief Calculate the matrix-matrix product.
 *
 * @param a - left-hand operand;
 * @param b - right-hand operand;
 * @param c - result;
 */
void mtx_mlt_mtx(struct mtx* a, struct mtx* b, struct mtx* c);

/**
 * @brief Calculate the matrix-vector product.
 *
 * @param a - left-hand operand;
 * @param b - right-hand operand;
 * @param c - result;
 */
void mtx_mlt_vec(struct mtx* a, struct vec* b, struct vec* c);

/**
 * @brief Calculate the matrix norm.
 *
 * @param a - operand;
 * @param r - result;
 */
void mtx_norm(struct mtx* a, real* r);

#endif  // MTX_DNS_H
