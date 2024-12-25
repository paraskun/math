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
  double x3 = v->data[2];

  return pow(sqrt(x1 * x1 + x2 * x2) - 2, 2) + x3 * x3 - 1;
}

double f2(struct vec* v) {
  double x1 = v->data[0];
  double x2 = v->data[1];
  double x3 = v->data[2];

  return (x1 - 2) * (x1 - 2) + x2 * x2 + x3 * x3 - 1;
}

double f3(struct vec* v) {
  double x1 = v->data[0];
  double x2 = v->data[1];
  double x3 = v->data[2];

  return pow(sqrt((x1 - 2) * (x1 - 2) + x2 * x2) - 0.7, 2) + x3 * x3 - 0.09;
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
  double x3 = itr->x->data[2];

  write(ctx->data, buf, sprintf(buf, "%.7lf %.7lf %.7lf\n", x1, x2, x3));
  fsync(ctx->data);

  fprintf(ctx->stat, "%d %.7e %.7e %.7e %.7e %.7e\n", itr->k, x1, x2, x3, itr->del, itr->err);

  fprintf(ctx->plot, "replot\n");
  fflush(ctx->plot);

  usleep(1000000 / 5);
}

int main() {
  struct ctx   ctx;
  struct pcut* fun;

  cut_new(&fun, 3, &f1, &f2, &f3);

  ctx.plot = popen("gnuplot -persist 2>/dev/null", "w");
  ctx.data = open("traj.dat", O_RDWR | O_CREAT | O_TRUNC, 0666);
  ctx.stat = fopen("stat.dat", "w+");

  fprintf(ctx.plot, "set parametric\n");
  fprintf(ctx.plot, "set view equal xyz\n");
  fprintf(ctx.plot, "set view ,,1.7\n");
  fprintf(ctx.plot, "splot (2 + 1*cos(u))*cos(v),(2 + 1*cos(u))*sin(v),sin(u) notitle, ");
  fprintf(ctx.plot, "cos(u)*cos(v)+2,cos(u)*sin(v),sin(u) notitle, ");
  fprintf(ctx.plot, "(0.7 + 0.3*cos(u))*cos(v)+2,(0.7 + 0.3*cos(u))*sin(v),0.3 * sin(u) notitle, ");
  fprintf(ctx.plot, "'traj.dat' using 1:2:3 with p pt 1 lc rgb 'red' notitle\n");
  fflush(ctx.plot);

  struct vcap    cap = cap(&cbk, &ctx);
  struct non_itr itr;
  struct vec*    x;

  vec_new(&x, 3);

  x->data[0] = 2;
  x->data[1] = 1;
  x->data[2] = 1;

  if (non_new_slv(
        fun,
        x,
        (struct non_opt){.mod = EXC,
                         .hem = 1000,
                         .eps = 1e-5,
                         .hop = 1e-3,
                         .itr = &itr,
                         .cbk = &cap,
                         .jac = nullptr}))
    fprintf(stderr, "%s\n", strerror(errno));

  close(ctx.data);
  fclose(ctx.stat);

  vec_cls(&x);
  cut_cls(&fun);

  fclose(ctx.plot);

  return 0;
}
