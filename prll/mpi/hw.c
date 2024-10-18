#include <errno.h>
#include <mpi.h>
#include <stdio.h>

#define COM MPI_COMM_WORLD

int main(int argc, char* argv[argc]) {
  if ((errno = MPI_Init(&argc, &argv)))
    goto err;

  int pc;
  int id;

  if ((errno = MPI_Comm_size(COM, &pc)))
    goto err;

  if ((errno = MPI_Comm_rank(COM, &id)))
    goto err;

  printf("Hello, I'm %d and there are %d of us!\n", id, pc);

  if ((errno = MPI_Finalize()))
    goto err;

  return 0;

err:
  MPI_Abort(COM, errno);
  return -1;
}
