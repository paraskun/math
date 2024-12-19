#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <vec/non.h>

char buf[255];

static FILE* plot;
static FILE* stat;

static int data;

double f1(struct vec* v) {
  double x1 = v->data[0];
  double x2 = v->data[1];

  return (x1 - 1) * (x1 - 1) + x2 * x2 - 1;
}

double f2(struct vec* v) {
  double x1 = v->data[0];
  double x2 = v->data[1];

  return (x1 + 1) * (x1 + 1) + x2 * x2 - 1;
}

double f3(struct vec* v) {
  double x1 = v->data[0];
  double x2 = v->data[1];

  return x1 - x2;
}

void cbk(struct non_res* res) {
  double x1 = res->x->data[0];
  double x2 = res->x->data[1];

  write(data, buf, sprintf(buf, "%.7lf %.7lf\n", x1, x2));
  fsync(data);

  fprintf(stat, "%d %.7e %.7e %.7e %.7e\n", res->k, x1, x2, res->del, res->err);

  fprintf(plot, "replot\n");
  fflush(plot);

  usleep(1000000 / 60);
}

double point[4][2] = {{2, 2}, {0, -3}, {-2, 0}};

int main() {
  for (int i = 0; i < 3; ++i) {
    sprintf(buf, "traj%d.dat", i + 1);
    close(open(buf, O_RDWR | O_TRUNC | O_CREAT, 0666));
  }

  double (*f[3])(struct vec*) = {&f1, &f2, &f3};

  plot = popen("gnuplot -persist", "w");

  fprintf(plot, "set parametric\n");
  fprintf(plot, "set xrange [-2.5:2.5]\n");
  fprintf(plot, "set yrange [-4:4]\n");
  fprintf(plot, "plot ");
  fprintf(plot, "cos(t) - 1,sin(t),");
  fprintf(plot, "cos(t) + 1,sin(t),");
  fprintf(plot, "t,t,");

  for (int i = 0; i < 3; ++i) {
    fprintf(plot, "'traj%d.dat' using 1:2 title '(%.2lf, %.2lf)'", i + 1, point[i][0], point[i][1]);
    fputc(i != 2 ? ',' : '\n', plot);
  }

  fflush(plot);

  struct non_res res;
  struct vec* x;

  vec_new(&x, 2);

  for (int i = 0; i < 3; ++i) {
    sprintf(buf, "traj%d.dat", i + 1);
    data = open(buf, O_RDWR);

    sprintf(buf, "stat%d.dat", i + 1);
    stat = fopen(buf, "w+");

    x->data[0] = point[i][0];
    x->data[1] = point[i][1];

    if (non_new_slv(3, f, x, (struct non_pps){
      .hem = 300,
      .eps = 1e-10,
      .hop = 0.0001,
      .res = &res,
      .cbk = &cbk
    })) {
      printf("%d: %s\n", i, strerror(errno));
      return -1;
    }

    close(data);
    fclose(stat);
  }

  vec_cls(&x);
  fclose(plot);

  return 0;
}
