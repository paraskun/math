#ifndef VEC_H
#define VEC_H

#include <stddef.h>
#include <stdio.h>

struct vec {
  real* v;
  int n;
};

/**
 * @brief Allocate memory and return a new vector with undefined values.
 *
 * @param n - vector dimension;
 */
struct vec* vec_new(int n);

/**
 * @brief Convert given vector to a new vector with randomly generated values. 
 *
 * @param vp - vector to convert;
 * @param u - values upper-bound;
 */
void vec_rnd(struct vec* vp, int u);

/**
 * @brief Convert given vector to a vector with sequential values started from zero.
 *
 * @param vp - vector to convert;
 */
void vec_seq(struct vec* vp);

/**
 * @brief Populate given vector with values from the file.
 *
 * @param fn - file name;
 * @param vp - vector to fill;
 */
void vec_fget(FILE* f, struct vec* vp);

/**
 * @brief Write the values from the given vector to a file.
 *
 * @param fn - file name;
 * @param vp - vector to write;
 */
void vec_fput(FILE* f, struct vec* vp);

/**
 * @brief Calculate the sum of two vectors.
 *
 * <p> r = a + b
 *
 * @param ap - left-hand operand;
 * @param bp - right-hand operand;
 * @param rp - result;
 */
void vec_add(struct vec* ap, struct vec* bp, struct vec* rp);

/**
 * @brief Calculate the linear combination of two vectors.
 *
 * <p> r = a + kb
 *
 * @param ap - left-hand operand;
 * @param bp - right-hand operand;
 * @param rp - result;
 * @param k - factor;
 */
void vec_cmb(struct vec* ap, struct vec* bp, struct vec* rp, real k);

/**
 * @brief Calculate the scalar product of two vectors.
 *
 * @param ap - left-hand operand;
 * @param bp - right-hand operand;
 * @param rp - result;
 */
void vec_mlt(struct vec* ap, struct vec* bp, real* rp);

/**
 * @brief Calculate the norm of the vector.
 *
 * @param vp - operand;
 * @param rp - result;
 */
void vec_nrm(struct vec* vp, double* rp);

/**
 * @brief Deallocate vector memory.
 *
 * @param vp - vector to delete;
 */
void vec_free(struct vec* vp);

#endif  // VEC_H
