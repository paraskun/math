#ifndef MTX_H
#define MTX_H

#include <def.h>
#include <vec/vec.h>

struct mtx;

/**
 * @brief Populate given matrix with values from the file.
 *
 * @param fn - file name;
 * @param m - matrix to fill;
 */
void mtx_fget(const char* fn, struct mtx* m);

/**
 * @brief Write the values from the given matrix to a file.
 *
 * @param fn - file name;

 * @param m - matrix to write;
 */
void mtx_fput(const char* fn, struct mtx* m);

/**
 * @brief Deallocate matrix memory.
 *
 * @param a - matrix to delete;
 */
void mtx_free(struct mtx* m);

#endif  // MTX_H
