#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <vec/non.h>

static char buf[255];

double f1(struct vec* v) {
  double x1 = v->data[0];
  double x2 = v->data[1];

  return cos(10 * x1) - x2;
}

double f2(struct vec* v) {
  double x1 = v->data[0];
  double x2 = v->data[1];

  return x2 - x1 - 1;
}

double f3(struct vec* v) {
  double x1 = v->data[0];
  double x2 = v->data[1];

  return x2 + x1 - 1;
}

struct ctx {
  FILE* plot;
  FILE* stat;

  int data;
};

void cbk(void* c, uint n, ...) {
  struct ctx* ctx = (struct ctx*)c;

  if (n < 1)
    return;

  va_list arg;
  va_start(arg, n);

  struct non_itr* itr = va_arg(arg, struct non_itr*);

  va_end(arg);

  double x1 = itr->x->data[0];
  double x2 = itr->x->data[1];

  write(ctx->data, buf, sprintf(buf, "%.7lf %.7lf\n", x1, x2));
  fsync(ctx->data);

  fprintf(ctx->stat, "%d %.7e %.7e %.7e %.7e\n", itr->k, x1, x2, itr->del, itr->err);

  fprintf(ctx->plot, "replot\n");
  fflush(ctx->plot);

  usleep(1000000 / 30);
}

int main() {
  const uint   num = 4;
  const double point[][2] = {{2, 2}, {0, -3}, {-2, 0}, {1.25, 0}};

  for (uint i = 0; i < num; ++i) {
    sprintf(buf, "traj%d.dat", i + 1);
    close(open(buf, O_RDWR | O_TRUNC | O_CREAT, 0666));
  }

  struct ctx   ctx;
  struct pcut* fun;

  cut_new(&fun, 3, &f1, &f2, &f3);

  ctx.plot = popen("gnuplot -persist 2>/dev/null", "w");

  fprintf(ctx.plot, "set parametric\n");
  fprintf(ctx.plot, "set samples 1000\n");
  fprintf(ctx.plot, "set xrange [-2.5:2.5]\n");
  fprintf(ctx.plot, "set yrange [-4:4]\n");
  fprintf(ctx.plot, "plot ");
  fprintf(ctx.plot, "t,cos(t*10),");
  fprintf(ctx.plot, "t,t+1,");
  fprintf(ctx.plot, "t,-t+1,");

  for (uint i = 0; i < num; ++i) {
    fprintf(
      ctx.plot, "'traj%d.dat' using 1:2 title '(%.2lf, %.2lf)'", i + 1, point[i][0], point[i][1]);
    fputc(i != num - 1 ? ',' : '\n', ctx.plot);
  }

  fflush(ctx.plot);

  struct vcap    cap = cap(&cbk, &ctx);
  struct non_itr itr;
  struct vec*    x;

  vec_new(&x, 2);

  for (uint i = 0; i < num; ++i) {
    sprintf(buf, "traj%d.dat", i + 1);
    ctx.data = open(buf, O_RDWR);

    sprintf(buf, "stat%d.dat", i + 1);
    ctx.stat = fopen(buf, "w+");

    x->data[0] = point[i][0];
    x->data[1] = point[i][1];

    if (non_new_slv(
          fun,
          x,
          (struct non_opt){.mod = CON,
                           .hem = 100,
                           .eps = 1e-5,
                           .hop = 1e-3,
                           .itr = &itr,
                           .cbk = &cap,
                           .jac = nullptr}))
      fprintf(stderr, "%d: %s\n", i + 1, strerror(errno));

    close(ctx.data);
    fclose(ctx.stat);
  }

  vec_cls(&x);
  cut_cls(&fun);

  fclose(ctx.plot);

  return 0;
}
