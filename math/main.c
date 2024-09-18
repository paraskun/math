#include <def.h>
#include <mtx_pfl.h>
#include <stdint.h>
#include <stdio.h>
#include "mtx.h"

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

  mtx_ldu(m);

  put(m);
  mtx_free(m);
  return 0;
}
