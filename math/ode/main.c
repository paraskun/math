#include "math/ode/euler.c"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define S 0.0
#define E 1.0
#define I 1.0

#define EPS 0.00001

f64 function(f64 t) {
  return exp(t * t);
}

f64 derivative(f64 t, f64 y) {
  return 2 * t * y;
}

point_t* guniform(f64 s, f64 e, f64 h) {
  u64 n = (e - s) / h;
  f64 t = s;

  point_t* grid = malloc(sizeof(point_t) * (n + 1));

  for (u64 i = 0; i <= n; ++i) {
    grid[i].t = t;
    grid[i].y = 0.0;

    t += h;
  }

  return grid;
}

void gprint(FILE* f, u64 n, point_t grid[n]) {
  for (u64 i = 0; i < n; ++i) {
    f64 a = function(grid[i].t);

    fprintf(f, "%.7e\t%.7e\t%.7e\t%.7e\n", grid[i].t, grid[i].y, a,
            fabs(a - grid[i].y));
  }
}

void gclear(point_t* grid) {
  free(grid);
}

int main() {
  f64 hs[] = {0.1, 0.05, 0.025};
  char name[] = "modx";

  for (int i = 0; i < 3; ++i) {
    name[3] = i + '0';

    FILE* f = fopen(name, "w");
    u64 n = (E - S) / hs[i] + 1;
    point_t* grid = guniform(S, E, hs[i]);

    grid[0].y = I;

    euler_impl_mod(&derivative, n, grid, EPS);
    gprint(f, n, grid);
    gclear(grid);
    fclose(f);
  }

  return 0;
}
