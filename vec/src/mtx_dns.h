#ifndef DMTX_H
#define DMTX_H

#include <stddef.h>
#include <stdio.h>
#include <vec.h>

struct mtx_dns {
  real* v;
  int n;
};

struct mtx_dns* mtx_dns_new(int n);

/**
 * @brief Populate given matrix with values from the file.
 *
 * @param fn - file name;
 * @param mp - matrix to fill;
 */
void mtx_dns_fget(FILE* f, struct mtx_dns* mp);

/**
 * @brief Write the values from the given matrix to a file.
 *
 * @param fn - file name;

 * @param mp - matrix to write;
 */
void mtx_dns_fput(FILE* f, struct mtx_dns* mp);

/**
 * @brief Convert given matrix to diagonally dominant matrix.
 *
 * @param mp - matrix to convert;
 * @param k - factor;
 */
void mtx_dns_ddm(struct mtx_dns* mp, int k);

/**
 * @brief Convert given matrix to Hilbert matrix.
 *
 * @param mp - matrix to convert;
 */
void mtx_dns_hlb(struct mtx_dns* mp);

/**
 * @brief Calculate the matrix-vector product.
 *
 * @param ap - left-hand operand;
 * @param bp - right-hand operand;
 * @param rp - result;
 */
void mtx_dns_vmlt(struct mtx_dns* ap, struct vec* bp, struct vec* rp);

/**
 * @brief Calculate the matrix-matrix product.
 *
 * @param ap - left-hand operand;
 * @param bp - right-hand operand;
 * @param cp - result;
 */
void mtx_dns_mmlt(struct mtx_dns* ap, struct mtx_dns* bp, struct mtx_dns* cp);

/**
 * @brief Convert given matrix to a matrix with randomly generated values.
 *
 * @param mp - matrix to convert;
 * @param u - values upper-bound;
 */
void mtx_dns_rnd(struct mtx_dns* mp, int u);

/**
 * @brief Calculate the matrix norm.
 *
 * @param ap - operand;
 * @param rp - result;
 */
void mtx_dns_nrm(struct mtx_dns* ap, real* rp);

/**
 * @brief Deallocate matrix memory.
 *
 * @param mp - matrix to delete;
 */
void mtx_dns_free(struct mtx_dns* mp);

#endif  // MTX_H
