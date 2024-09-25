#ifndef MTX_H
#define MTX_H

#include <def.h>
#include <vec/vec.h>

#include <stdio.h>

struct mtx;

/**
 * @brief Populate given matrix with values from the file.
 *
 * @param fn - file name;
 * @param m - matrix to fill;
 */
void mtx_fget(FILE* f, struct mtx* mp);

/**
 * @brief Write the values from the given matrix to a file.
 *
 * @param fn - file name;

 * @param m - matrix to write;
 */
void mtx_fput(FILE* f, struct mtx* mp);

/**
 * @brief Convert given matrix to diagonally dominant matrix.
 *
 * @param mp - matrix to convert;
 * @param k - factor;
 */
void mtx_ddm(struct mtx* mp, size_t k);

/**
 * @brief Convert given matrix to Hilbert matrix.
 *
 * @param mp - matrix to convert;
 */
void mtx_hlb(struct mtx* mp);

/**
 * @brief Deallocate matrix memory.
 *
 * @param a - matrix to delete;
 */
void mtx_free(struct mtx* mp);

#endif  // MTX_H
