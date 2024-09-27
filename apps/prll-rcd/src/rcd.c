#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <vec.h>

#define N 100000

#define TAG 0
#define DBL MPI_DOUBLE
#define COM MPI_COMM_WORLD
#define ANY MPI_ANY_SOURCE
#define ROOT 0

void do_root(int pc, int id);
void do_work(int pc, int id);

int main(int argc, char* argv[argc]) {
  int e;
  int pc;
  int id;

  if ((e = MPI_Init(&argc, &argv))) {
    MPI_Abort(MPI_COMM_WORLD, e);
    exit(-1);
  }

  MPI_Comm_size(MPI_COMM_WORLD, &pc);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);

  if (id == ROOT)
    do_root(pc, id);
  else
    do_work(pc, id);

  MPI_Finalize();

  return 0;
}

void do_root(int pc, int id) {
  int e;
  MPI_Status stat;
  double nrm;
  char buf[64];
  time_t raw;

  for (int i = 0; i < pc - 1; ++i) {
    if ((e = MPI_Recv(&nrm, 1, DBL, ANY, TAG, COM, &stat))) {
      MPI_Abort(COM, e);
      exit(-1);
    }

    raw = time(0);
    strftime(buf, 64, "%T", localtime(&raw));
    printf("%lf [%d] --- %s\n", nrm, stat.MPI_SOURCE, buf);
  }
}

void do_work(int pc, int id) {
  srand(time(NULL) + id);

  struct vec* v = vec_new(N);
  double nrm;
  int e;

  vec_rnd(v, 100);
  vec_nrm(v, &nrm);

  if ((e = MPI_Send(&nrm, 1, DBL, ROOT, TAG, COM))) {
    MPI_Abort(COM, e);
    exit(-1);
  }
}
