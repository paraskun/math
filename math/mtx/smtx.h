#ifndef SMTX_H

#include <mtx/mtx.h>

#include <stdint.h>

/**
 * @class mtx
 * @brief Banded matrix storage.
 */
struct mtx {
  real* d;
  real* l;
  real* u;

  size_t* p;

  size_t n;
  size_t s;
};

/**
 * @brief Allocate memory and return a new matrix with undefined values.
 *
 * @param n - matrix dimensions;
 * @param s - number of "non-zero" elements;
 */
struct mtx* mtx_new(size_t n, size_t s);

/**
 * @brief Perform LDU decomposition of the given matrix.
 *
 * @param m - matrix for decomposition;
 */
void mtx_ldu(struct mtx* mp);

#endif  // MTX_PFL_H
