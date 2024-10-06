#include <errno.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mtx.h>
#include <vec.h>

#define TAG 0
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

  if (pc > n)
    pc = n;

  int rc = n / pc;
  int rf = 0;

  if (id == ROOT)
    rc = rc + (n - rc * pc);
  else
    rf = rc + (n - rc * pc) + (id - 1) * rc;

  if (!(mp = pmtx_new(n, rf, rc)))
    goto end;

  if (!(bp = vec_new(n)))
    goto end;

  if (!(xp = vec_new(n)))
    goto end;

  if (!(fmtx = fopen("pmtx.in", "r")))
    goto end;

  if (!(fx = fopen("x.in", "r")))
    goto end;

  if (id == ROOT) {
    MPI_Request* req = malloc(sizeof(MPI_Request) * (pc - 1));

    if (!req) {
      errno = ENOMEM;
      goto end;
    }

    for (int i = 1; i < pc; ++i) {
      int c = n / pc;
      int f = rc + (i - 1) * c;

      if ((errno = MPI_Irecv(bp->v + f, c, DBL, i, TAG, COM, req))) {
        free(req);
        goto end;
      }
    }

    mtx_fget(fmtx, mp);
    vec_fget(fx, xp);
    mtx_vmlt(mp, xp, bp);

    if ((errno = MPI_Waitall(pc - 1, req, NULL))) {
      free(req);
      goto end;
    }

    free(req);

    FILE* fb = fopen("b.out", "w+");

    if (!fb)
      goto end;

    vec_fput(fb, bp);

    fclose(fb);
  } else if (id < pc) {
    mtx_fget(fmtx, mp);
    vec_fget(fx, xp);
    mtx_vmlt(mp, xp, bp);

    if ((errno = MPI_Send(bp->v + rf, rc, DBL, ROOT, TAG, COM)))
      goto end;
  }

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
    printf("fatal: %s.\n", strerror(errno));

    MPI_Abort(COM, errno);
    return -1;
  }

  MPI_Finalize();
  return 0;
}
