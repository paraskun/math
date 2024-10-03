#include <errno.h>
#include <mpi.h>
#include <stdio.h>
#include <string.h>

#include <mtx.h>
#include <vec.h>

#define ROOT 0
#define COM MPI_COMM_WORLD
#define DBL MPI_DOUBLE

int main(int argc, char* argv[argc]) {
  if ((errno = MPI_Init(&argc, &argv))) {
    MPI_Abort(COM, errno);
    return -1;
  }

  int pc;
  int id;

  MPI_Comm_size(COM, &pc);
  MPI_Comm_rank(COM, &id);

  FILE* fdms = 0;
  FILE* fmtx = 0;
  FILE* fx = 0;

  struct pmtx* mp = 0;

  struct vec* bp = 0;
  struct vec* xp = 0;

  if (!(fdms = fopen("dms.in", "r")))
    goto end;

  int n;

  if (fscanf(fdms, "%d", &n) != 1)
    goto end;

  // if (!(mp = pmtx_new(n)))
  //   goto end;

  if (!(bp = vec_new(n)))
    goto end;

  if (!(xp = vec_new(n)))
    goto end;

  if (!(fmtx = fopen("dmtx.in", "r")))
    goto end;

  if (!(fx = fopen("x.in", "r")))
    goto end;

  mtx_fget(fmtx, mp);
  vec_fget(fx, xp);

end:
  if (mp)
    mtx_free(mp);

  if (bp)
    vec_free(bp);

  if (xp)
    vec_free(xp);

  if (fdms)
    fclose(fdms);

  if (fmtx)
    fclose(fmtx);

  if (fx)
    fclose(fx);

  if (errno) {
    if (id == ROOT)
      printf("fatal: %s.\n", strerror(errno));

    MPI_Abort(COM, errno);
    return -1;
  }

  MPI_Finalize();
  return 0;
}
