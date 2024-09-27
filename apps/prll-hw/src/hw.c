#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[argc]) {
  int e;

  if ((e = MPI_Init(&argc, &argv))) {
    MPI_Abort(MPI_COMM_WORLD, e);
    exit(-1);
  }

  int pc;
  int id;

  MPI_Comm_size(MPI_COMM_WORLD, &pc);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);

  printf("Hello, I'm %d and there are %d of us!\n", id, pc);

  MPI_Finalize();

  return 0;
}
