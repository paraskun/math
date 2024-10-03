#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define COM MPI_COMM_WORLD

int main(int argc, char* argv[argc]) {
  int e;

  if ((e = MPI_Init(&argc, &argv))) {
    MPI_Abort(COM, e);
    exit(-1);
  }

  int pc;
  int id;

  MPI_Comm_size(COM, &pc);
  MPI_Comm_rank(COM, &id);

  printf("Hello, I'm %d and there are %d of us!\n", id, pc);

  MPI_Finalize();

  return 0;
}
