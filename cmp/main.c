#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "vec/vec.h"

#define N 2500

int main() {
  srand(time(NULL));

  struct vec *v1 = vec_seq(3);
  struct vec *v2 = vec_seq(3);

  double product = 0;

  vec_mlt(v1, v2, &product);

  printf("%lf\n", product);

  return 0;
}
