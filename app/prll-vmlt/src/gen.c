#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[argc]) {
  size_t n = strtoul(argv[1], NULL, 10);

  FILE* fd = fopen("dms.in", "w+");
  FILE* fm = fopen("pmtx.in", "w+");
  FILE* fx = fopen("x.in", "w+");

  for (size_t i = 0, s = 1; i < n; ++i) {
    fprintf(fx, "%lu ", i + 1);

    for (size_t j = 0; j < n - 1; ++j, ++s)
      fprintf(fm, "%9lu ", s);

    fprintf(fm, "%9lu\n", s++);
  }

  fprintf(fd, "%lu", n);

  fclose(fd);
  fclose(fm);
  fclose(fx);

  return 0;
}
