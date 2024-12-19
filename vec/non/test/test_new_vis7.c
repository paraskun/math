#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <vec/non.h>
#include <math.h>

#define HEM 1000
#define EPS 1e-5

static int dd;
static FILE* gp;
static FILE* st;

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

void cbk(struct non_res* res) {
  char buf[255];

  double x1 = res->x->data[0];
  double x2 = res->x->data[1];
  double x3 = res->x->data[2];

  write(dd, buf, sprintf(buf, "%.7lf %.7lf %.7lf\n", x1, x2, x3));
  fsync(dd);

  fprintf(gp, "replot\n");
  fflush(gp);

  fprintf(st, "%d %.7lf %.7lf %.7lf %.7lf %.7lf\n", res->k, x1, x2, x3, res->del, res->err);

  usleep(1000000 / 2);
}

int main() {
  double (*f[3])(struct vec*) = {&f1, &f2, &f3};

  dd = open("traj.dat", O_RDWR | O_CREAT | O_TRUNC);
  gp = popen("gnuplot -persist", "w");
  st = fopen("stat.dat", "w+");

  fprintf(gp, "set parametric\n");
  fprintf(gp, "set view equal xyz\n");
  fprintf(gp, "set view ,,1.7\n");
  fprintf(gp, "splot (2 + 1*cos(u))*cos(v),(2 + 1*cos(u))*sin(v),sin(u) notitle, ");
  fprintf(gp, "cos(u)*cos(v)+2,cos(u)*sin(v),sin(u) notitle, ");
  fprintf(gp, "(0.7 + 0.3*cos(u))*cos(v)+2,(0.7 + 0.3*cos(u))*sin(v),0.3 * sin(u) notitle, ");
  fprintf(gp, "'traj.dat' using 1:2:3 with points pointtype 1 notitle\n");
  fflush(gp);

  struct vec* x;

  vec_new(&x, 3);

  x->data[0] = 2;
  x->data[1] = 1;
  x->data[2] = 1;

  struct non_res res;

  non_new_slv(3, f, x, (struct non_pps){
    .res = &res,
    .cbk = &cbk,
    .hop = 0.001,
    .eps = 0.001,
    .hem = 1000
  });

  close(dd);
  fclose(gp);
  fclose(st);

  vec_cls(&x);

  return 0;
}
