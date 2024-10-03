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

void recv(int pc);
void send(int id);

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
    recv(pc);
  else
    send(id);

  MPI_Finalize();

  return 0;
}

void recv(int pc) {
  MPI_Status stat;
  double nrm;

  char buf[64];
  time_t epoch;

  for (int i = 0, e; i < pc - 1; ++i) {
    if ((e = MPI_Recv(&nrm, 1, DBL, ANY, TAG, COM, &stat))) {
      MPI_Abort(COM, e);
      exit(-1);
    }

    epoch = time(0);

    strftime(buf, 64, "%T", localtime(&epoch));
    printf("%lf [%d] --- %s\n", nrm, stat.MPI_SOURCE, buf);
  }
}

void send(int id) {
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
