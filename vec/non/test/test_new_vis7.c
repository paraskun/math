#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <vec/non.h>

char buf[255];

static FILE* plot;
static FILE* stat;

static int data;

double f1(struct vec* v) {
  double x1 = v->data[0];
  double x2 = v->data[1];

  return sin(10 * x1 + M_PI / 2) - x2;
}

double f2(struct vec* v) {
  double x1 = v->data[0];
  double x2 = v->data[1];

  return x2 - x1 - 1;
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

double point[4][2] = {{2, 2}, {0, -3}, {-2, 0}, {1.25, 0}};

int main() {
  for (int i = 0; i < 3; ++i) {
    sprintf(buf, "traj%d.dat", i + 1);
    close(open(buf, O_RDWR | O_TRUNC | O_CREAT, 0666));
  }

  double (*f[2])(struct vec*) = {&f1, &f2};

  plot = popen("gnuplot -persist", "w");

  fprintf(plot, "set parametric\n");
  fprintf(plot, "set samples 1000\n");
  fprintf(plot, "set xrange [-2.5:2.5]\n");
  fprintf(plot, "set yrange [-4:4]\n");
  fprintf(plot, "plot ");
  fprintf(plot, "t,sin(t*10 + pi/2),");
  fprintf(plot, "t-1,t,");

  for (int i = 0; i < 4; ++i) {
    fprintf(plot, "'traj%d.dat' using 1:2 title '(%.2lf, %.2lf)'", i + 1, point[i][0], point[i][1]);
    fputc(i != 3 ? ',' : '\n', plot);
  }

  fflush(plot);

  struct non_res res;
  struct vec* x;

  vec_new(&x, 2);

  for (int i = 0; i < 4; ++i) {
    sprintf(buf, "traj%d.dat", i + 1);
    data = open(buf, O_RDWR);

    sprintf(buf, "stat%d.dat", i + 1);
    stat = fopen(buf, "w+");

    x->data[0] = point[i][0];
    x->data[1] = point[i][1];

    if (non_new_slv(2, f, x, (struct non_pps){
      .hem = 300,
      .eps = 1e-15,
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
