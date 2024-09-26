#include <hw.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void hello(int argc, char* argv[argc]) {
  int n = atoi(argv[1]);
  int e;

  if ((e = MPI_Init(&argc, &argv))) {
    MPI_Abort(MPI_COMM_WORLD, e);
    exit(-1);
  }

  int pc;
  int id;

  MPI_Comm_size(MPI_COMM_WORLD, &pc);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);

  printf("Hello, I'm %d!", id);

  MPI_Finalize();
}
