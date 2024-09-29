#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <vec.h>

#define N 100000

#define TAG 0
#define DBL MPI_DOUBLE
#define COM MPI_COMM_WORLD
#define ROOT 0

void do_root(int pc, int id);
void do_work(int pc, int id);

int main(int argc, char* argv[argc]) {
  int e;
  int pc;
  int id;

  if ((e = MPI_Init(&argc, &argv))) {
    MPI_Abort(COM, e);
    exit(-1);
  }

  MPI_Comm_size(COM, &pc);
  MPI_Comm_rank(COM, &id);

  if (id == ROOT)
    do_root(pc, id);
  else
    do_work(pc, id);

  MPI_Finalize();

  return 0;
}

void do_root(int pc, int id) {
  MPI_Request req[pc - 1];
  double nrm[pc - 1];
  int f[pc - 1];
  int e;

  for (int i = 1; i < pc; ++i) {
    f[i - 1] = 0;

    if ((e = MPI_Irecv(&nrm[i - 1], 1, DBL, i, TAG, COM, &req[i - 1]))) {
      MPI_Abort(COM, e);
      exit(-1);
    }
  }

  int s = 0;

  while (s != pc - 1)
    for (int i = 0; i < pc - 1; ++i)
      if (!f[i]) {
        if ((e = MPI_Test(&req[i], &f[i], NULL))) {
          MPI_Abort(COM, e);
          exit(-1);
        }

        if (f[i]) {
          s += 1;
          printf("Norm from %d: %lf;\n", i + 1, nrm[i]);
        }
      }
}

void do_work(int pc, int id) {
  srand(time(NULL) + id);

  struct vec* v = vec_new(N);
  double nrm;
  int e;

  vec_rnd(v, 100);
  vec_nrm(v, &nrm);

  if (id == 1)
    sleep(10);

  if ((e = MPI_Send(&nrm, 1, DBL, 0, TAG, COM))) {
    MPI_Abort(COM, e);
    exit(-1);
  }
}
