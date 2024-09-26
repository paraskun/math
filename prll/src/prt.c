#include <mpi.h>
#include <prt.h>
#include <unistd.h>
#include <vec.h>

#define N 100000
#define U 100

void print(int pc, int id) {
  if (id == 0) {
  } else {
    struct vec* v = vec_new(N);
    real nrm;

    vec_rnd(v, U);
    vec_norm(v, &nrm);

    if (id == 1)
      sleep(10);

    MPI_Send(&nrm, sizeof(real), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

    vec_free(v);
  }
}
