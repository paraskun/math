#include <def.h>
#include <mtx_pfl.h>
#include <stdint.h>
#include <stdio.h>
#include "mtx.h"

struct mtx* get() {
  FILE* in = fopen("in", "r");
  uint32_t n, s;

  fscanf(in, "%u", &n);
  fscanf(in, "%u", &s);

  struct mtx* m = mtx_new(n, s);
  mtx_fget(in, m);

  fclose(in);
  return m;
}

void put(struct mtx* m) {
  FILE* out = fopen("out", "w");

  fprintf(out, "%u %u\n", m->n, m->s);
  mtx_fput(out, m);

  fclose(out);
}

int main() {
  struct mtx* m = get();
  put(m);

  mtx_free(m);
  return 0;
}
