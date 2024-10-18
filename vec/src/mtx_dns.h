#ifndef DMTX_H
#define DMTX_H

#include <stddef.h>
#include <stdio.h>
#include <vec.h>

struct dmtx {
  real* v;
  int n;
};

struct dmtx* dmtx_new(int n);

/**
 * @brief Populate given matrix with values from the file.
 *
 * @param fn - file name;
 * @param mp - matrix to fill;
 */
void dmtx_fget(FILE* f, struct dmtx* mp);

/**
 * @brief Write the values from the given matrix to a file.
 *
 * @param fn - file name;

 * @param mp - matrix to write;
 */
void dmtx_fput(FILE* f, struct dmtx* mp);

/**
 * @brief Convert given matrix to diagonally dominant matrix.
 *
 * @param mp - matrix to convert;
 * @param k - factor;
 */
void dmtx_ddm(struct dmtx* mp, int k);

/**
 * @brief Convert given matrix to Hilbert matrix.
 *
 * @param mp - matrix to convert;
 */
void dmtx_hlb(struct dmtx* mp);

/**
 * @brief Calculate the matrix-vector product.
 *
 * @param ap - left-hand operand;
 * @param bp - right-hand operand;
 * @param rp - result;
 */
void dmtx_vmlt(struct dmtx* ap, struct vec* bp, struct vec* rp);

/**
 * @brief Calculate the matrix-matrix product.
 *
 * @param ap - left-hand operand;
 * @param bp - right-hand operand;
 * @param cp - result;
 */
void dmtx_mmlt(struct dmtx* ap, struct dmtx* bp, struct dmtx* cp);

/**
 * @brief Convert given matrix to a matrix with randomly generated values.
 *
 * @param mp - matrix to convert;
 * @param u - values upper-bound;
 */
void dmtx_rnd(struct dmtx* mp, int u);

/**
 * @brief Calculate the matrix norm.
 *
 * @param ap - operand;
 * @param rp - result;
 */
void dmtx_nrm(struct dmtx* ap, real* rp);

/**
 * @brief Deallocate matrix memory.
 *
 * @param mp - matrix to delete;
 */
void dmtx_free(struct dmtx* mp);

#endif  // MTX_H
