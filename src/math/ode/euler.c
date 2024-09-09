#include <stddef.h>

typedef double f64;
typedef size_t u64;

typedef enum method { SPL, MOD, EXT } method_t;

typedef struct point {
  f64 t;
  f64 y;
} point_t;

void euler(f64 (*d)(f64, f64), u64 n, point_t grid[n], method_t m) {
  for (u64 i = 1; i < n; ++i) {
    f64 h = grid[i].t - grid[i - 1].t;

    point_t* c = &grid[i];
    point_t* p = &grid[i - 1];

    switch (m) {
      case SPL:
        c->y = p->y + h * d(p->t, p->y);
      case MOD:
        c->y =
            p->y + h / 2 * (d(p->t, p->t) + d(c->t, p->y + h * d(p->t, p->y)));
      case EXT:
        c->y = p->y + h * d(grid[i / 2].t, p->y + h / 2 * d(p->t, p->y));
    }
  }
}

