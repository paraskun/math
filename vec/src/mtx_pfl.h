#ifndef SMTX_H
#define SMTX_H

#include <vec.h>

#include <stddef.h>
#include <stdio.h>

struct mtx_pfl {
  real* d;
  real* l;
  real* u;

  int* p;

  int n;
  int s;
};

struct mtx_pfl* mtx_pfl_new(int n, int s);

/**
 * @brief Perform matrix LDU decomposition.
 *
 * @param mp - matrix to decompose;
 */
void mtx_pfl_ldu(struct mtx_pfl* mp);

/**
 * @brief Populate given matrix with values from the file.
 *
 * @param fn - file name;
 * @param mp - matrix to fill;
 */
void mtx_pfl_fget(FILE* f, struct mtx_pfl* mp);

/**
 * @brief Write the values from the given matrix to a file.
 *
 * @param fn - file name;

 * @param mp - matrix to write;
 */
void mtx_pfl_fput(FILE* f, struct mtx_pfl* mp);

/**
 * @brief Convert given matrix to diagonally dominant matrix.
 *
 * @param mp - matrix to convert;
 * @param k - factor;
 */
void mtx_pfl_ddm(struct mtx_pfl* mp, int k);

/**
 * @brief Convert given matrix to Hilbert matrix.
 *
 * @param mp - matrix to convert;
 */
void mtx_pfl_hlb(struct mtx_pfl* mp);

/**
 * @brief Calculate the matrix-vector product.
 *
 * @param ap - left-hand operand;
 * @param bp - right-hand operand;
 * @param rp - result;
 */
void mtx_pfl_vmlt(struct mtx_pfl* ap, struct vec* bp, struct vec* rp);

/**
 * @brief Deallocate matrix memory.
 *
 * @param mp - matrix to delete;
 */
void mtx_pfl_free(struct mtx_pfl* mp);

#endif  // SMTX_H
