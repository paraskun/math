#ifndef SMTX_H
#define SMTX_H

#include <vec.h>

#include <stddef.h>
#include <stdio.h>

struct smtx {
  real* d;
  real* l;
  real* u;

  int* p;

  int n;
  int s;
};

struct smtx* smtx_new(int n, int s);

/**
 * @brief Perform matrix LDU decomposition.
 *
 * @param mp - matrix to decompose;
 */
void smtx_ldu(struct smtx* mp);

/**
 * @brief Populate given matrix with values from the file.
 *
 * @param fn - file name;
 * @param mp - matrix to fill;
 */
void smtx_fget(FILE* f, struct smtx* mp);

/**
 * @brief Write the values from the given matrix to a file.
 *
 * @param fn - file name;

 * @param mp - matrix to write;
 */
void smtx_fput(FILE* f, struct smtx* mp);

/**
 * @brief Convert given matrix to diagonally dominant matrix.
 *
 * @param mp - matrix to convert;
 * @param k - factor;
 */
void smtx_ddm(struct smtx* mp, int k);

/**
 * @brief Convert given matrix to Hilbert matrix.
 *
 * @param mp - matrix to convert;
 */
void smtx_hlb(struct smtx* mp);

/**
 * @brief Calculate the matrix-vector product.
 *
 * @param ap - left-hand operand;
 * @param bp - right-hand operand;
 * @param rp - result;
 */
void smtx_vmlt(struct smtx* ap, struct vec* bp, struct vec* rp);

/**
 * @brief Deallocate matrix memory.
 *
 * @param mp - matrix to delete;
 */
void smtx_free(struct smtx* mp);

#endif  // SMTX_H
