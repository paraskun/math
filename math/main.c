#include <def.h>
#include <mtx_pfl.h>
#include <sle.h>
#include <stdint.h>
#include <stdio.h>

struct mtx* get() {
  FILE* in = fopen("in", "r");
  uint32_t n, s;

  fscanf(in, "%u %u", &n, &s);

  struct mtx* m = mtx_new(n, s);
  mtx_fget(in, m);

  fclose(in);
  return m;
}

void put(struct mtx* m) {
  FILE* out = fopen("out", "w");
  mtx_fput(out, m);
  fclose(out);
}

int main() {
  struct mtx* m = get();

  struct vec* x = vec_new(m->n);
  struct vec* b = vec_seq(m->n);

  sle_gauss(m, x, b);

  put(m);

  vec_cput(x);
  vec_cput(b);

  mtx_free(m);
  vec_free(x);
  vec_free(b);

  return 0;
}
