#include <math.h>
#include <stddef.h>

typedef double f64;
typedef size_t u64;

typedef struct point {
  f64 t;
  f64 y;
} point_t;

void euler_smp(f64 (*d)(f64, f64), u64 n, point_t grid[n]) {
  for (u64 i = 1; i < n; ++i) {
    f64 h = grid[i].t - grid[i - 1].t;

    grid[i].y = grid[i - 1].y + h * d(grid[i - 1].t, grid[i - 1].y);
  }
}

void euler_mod(f64 (*d)(f64, f64), u64 n, point_t grid[n]) {
  for (u64 i = 1; i < n; ++i) {
    f64 h = grid[i].t - grid[i - 1].t;

    grid[i].y =
        grid[i - 1].y +
        h / 2 *
            (d(grid[i - 1].t, grid[i - 1].y) +
             d(grid[i].t, grid[i - 1].y + h * d(grid[i - 1].t, grid[i - 1].y)));
  }
}

void euler_ext(f64 (*d)(f64, f64), u64 n, point_t grid[n]) {
  for (u64 i = 1; i < n; ++i) {
    f64 h = grid[i].t - grid[i - 1].t;

    grid[i].y = grid[i - 1].y +
                h * d(grid[i / 2].t,
                      grid[i - 1].y + h / 2 * d(grid[i - 1].t, grid[i - 1].y));
  }
}

void euler_impl_smp(f64 (*d)(f64, f64), u64 n, point_t grid[n], f64 eps) {
  for (u64 i = 1; i < n; ++i) {
    f64 h = grid[i].t - grid[i - 1].t;
    f64 p = grid[i - 1].y + h * d(grid[i - 1].t, grid[i - 1].y);

    do {
      grid[i].y = grid[i - 1].y + h * d(grid[i].t, p);

      if (fabs(p - grid[i].y) < eps)
        break;
      else
        p = grid[i].y;
    } while (1);
  }
}

void euler_impl_mod(f64 (*d)(f64, f64), u64 n, point_t grid[n], f64 eps) {
  for (u64 i = 1; i < n; ++i) {
    f64 h = grid[i].t - grid[i - 1].t;
    f64 p = grid[i - 1].y + h * d(grid[i - 1].t, grid[i - 1].y);

    do {
      grid[i].y = grid[i - 1].y +
                  h / 2 * (d(grid[i - 1].t, grid[i - 1].y) + d(grid[i].t, p));

      if (fabs(p - grid[i].y) < eps)
        break;
      else
        p = grid[i].y;
    } while (1);
  }
}
