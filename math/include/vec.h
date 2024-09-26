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
 * @param v - vector to convert;
 * @param u - values upper-bound;
 */
void vec_rnd(struct vec* v, int u);

/**
 * @brief Convert given vector to a vector with sequential values started from zero.
 *
 * @param v - vector to convert;
 */
void vec_seq(struct vec* v);

/**
 * @brief Populate given vector with values from the file.
 *
 * @param fn - file name;
 * @param a - vector to fill;
 */
void vec_fget(FILE* f, struct vec* a);

/**
 * @brief Write the values from the given vector to a file.
 *
 * @param fn - file name;

 * @param a - vector to write;
 */
void vec_fput(FILE* f, struct vec* a);

/**
 * @brief Calculate the sum of two vectors.
 *
 * <p> c = a + b
 *
 * @param a - left-hand operand;
 * @param b - right-hand operand;
 * @param r - result;
 */
void vec_add(struct vec* a, struct vec* b, struct vec* r);

/**
 * @brief Calculate the linear combination of two vectors.
 *
 * <p> c = a + kb
 *
 * @param a - left-hand operand;
 * @param b - right-hand operand;
 * @param c - result;
 * @param k - factor;
 */
void vec_cmb(struct vec* a, struct vec* b, struct vec* c, real k);

/**
 * @brief Calculate the scalar product of two vectors.
 *
 * @param a - left-hand operand;
 * @param b - right-hand operand;
 * @param r - result;
 */
void vec_mlt(struct vec* a, struct vec* b, real* r);

/**
 * @brief Calculate the norm of the vector.
 *
 * @param a - operand;
 * @param r - result;
 */
void vec_norm(struct vec* a, real* r);

/**
 * @brief Deallocate vector memory.
 *
 * @param a - vector to delete;
 */
void vec_free(struct vec* a);

#endif  // VEC_H
