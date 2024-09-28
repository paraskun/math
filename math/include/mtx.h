#ifndef MTX_H
#define MTX_H

#include <stddef.h>
#include <stdio.h>
#include <vec.h>

struct mtx;

/**
 * @brief Populate given matrix with values from the file.
 *
 * @param fn - file name;
 * @param mp - matrix to fill;
 */
void mtx_fget(FILE* f, struct mtx* mp);

/**
 * @brief Write the values from the given matrix to a file.
 *
 * @param fn - file name;

 * @param mp - matrix to write;
 */
void mtx_fput(FILE* f, struct mtx* mp);

/**
 * @brief Convert given matrix to diagonally dominant matrix.
 *
 * @param mp - matrix to convert;
 * @param k - factor;
 */
void mtx_ddm(struct mtx* mp, int k);

/**
 * @brief Convert given matrix to Hilbert matrix.
 *
 * @param mp - matrix to convert;
 */
void mtx_hlb(struct mtx* mp);

/**
 * @brief Deallocate matrix memory.
 *
 * @param mp - matrix to delete;
 */
void mtx_free(struct mtx* mp);

#ifdef SMTX

/**
 * @class mtx
 * @brief Banded matrix storage.
 */
struct mtx {
  real* d;
  real* l;
  real* u;

  int* p;

  int n;
  int s;
};

/**
 * @brief Allocate memory and return a new matrix with undefined values.
 *
 * @param n - matrix dimensions;
 * @param s - number of "non-zero" elements;
 */
struct mtx* mtx_new(int n, int s);

/**
 * @brief Perform LDU decomposition of the given matrix.
 *
 * @param mp - matrix for decomposition;
 */
void mtx_ldu(struct mtx* mp);

#else

/**
 * @class mtx
 * @brief Dense matrix storage.
 */
struct mtx {
  real* v;
  int n;
};

/**
 * @brief Allocate memory and return a new matrix with undefined values.
 *
 * @param n - matrix dimensions;
 */
struct mtx* mtx_new(int n);

/**
 * @brief Convert given matrix to a matrix with randomly generated values. 
 *
 * @param mp - matrix to convert;
 * @param u - values upper-bound;
 */
void mtx_rnd(struct mtx* mp, int u);

/**
 * @brief Convert given matrix to a new matrix with sequential values starting from 1.
 *
 * @param mp - matrix dimensions;
 */
void mtx_seq(struct mtx* mp);

/**
 * @brief Calculate the matrix-matrix product.
 *
 * @param ap - left-hand operand;
 * @param bp - right-hand operand;
 * @param cp - result;
 */
void mtx_mmlt(struct mtx* ap, struct mtx* bp, struct mtx* cp);

/**
 * @brief Calculate the matrix-vector product.
 *
 * @param ap - left-hand operand;
 * @param bp - right-hand operand;
 * @param cp - result;
 */
void mtx_vmlt(struct mtx* ap, struct vec* bp, struct vec* cp);

/**
 * @brief Calculate the matrix norm.
 *
 * @param ap - operand;
 * @param rp - result;
 */
void mtx_norm(struct mtx* ap, real* rp);

#endif  // SMTX
#endif  // MTX_H
