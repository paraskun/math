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
#define INT MPI_INT

#define N 5

int main(int argc, char* argv[argc]) {

  if ((errno = MPI_Init(&argc, &argv))) {
    MPI_Abort(COM, errno);
    return -1;
  }

  int pc;
  int id;

  if ((errno = MPI_Comm_size(COM, &pc))) {
    MPI_Abort(COM, errno);
    return -1;
  }

  if ((errno = MPI_Comm_rank(COM, &id))) {
    MPI_Abort(COM, errno);
    return -1;
  }

  struct vec* xp = 0;
  struct vec* fp = 0;
  struct pmtx* mp = 0;

  int* t = 0;
  int* td = 0;
  int* tc = 0;

  int* vd = 0;
  int* vc = 0;

  int* fd = 0;

  if (id == ROOT) {
    t = malloc(sizeof(int) * pc);

    if (!t)
      goto end;

    td = malloc(sizeof(int) * pc);

    if (!td)
      goto end;

    tc = malloc(sizeof(int) * pc);

    if (!tc)
      goto end;

    vd = malloc(sizeof(int) * pc);

    if (!vd)
      goto end;

    vc = malloc(sizeof(int) * pc);

    if (!vc)
      goto end;

    fd = malloc(sizeof(int) * pc);

    if (!fd)
      goto end;

    for (int i = 0; i < pc; ++i) {
      t[i] = 0;
      td[i] = i;
      tc[i] = 1;
      vc[i] = 0;
      fd[i] = 0;
    }
  } else {
    t = malloc(sizeof(int));

    if (!t)
      goto end;
  }

  int p = N < pc ? N : pc;

  if (id == ROOT)
    for (int i = p - 1, s = N; i >= 0; --i) {
      t[td[i]] = s / (i + 1);
      s = s - t[td[i]];

      vc[i] = t[td[i]] * N;
      vd[i] = s * N;

      fd[i] = s;
    }

  errno = MPI_Scatterv(t, tc, td, INT, t, 1, INT, ROOT, COM);

  if (errno)
    goto end;

  mp = pmtx_new(N, 0, id == ROOT ? N : *t);

  if (!mp)
    goto end;

  if (id == ROOT)
    pmtx_seq(mp, 1);

  errno = MPI_Scatterv(mp->v, vc, vd, DBL, mp->v, *t * N, DBL, ROOT, COM);

  if (errno)
    goto end;

  if (id == ROOT) {
    mp->c = *t;
  }

  xp = vec_new(N);

  if (!xp)
    goto end;

  if (id == ROOT)
    vec_seq(xp, 1);

  errno = MPI_Bcast(xp->v, N, DBL, ROOT, COM);

  if (errno)
    goto end;

  fp = vec_new(N);

  if (!fp)
    goto end;

  if (mtx_vmlt(mp, xp, fp))
    goto end;

  errno = MPI_Gatherv(fp->v, *t, DBL, fp->v, t, fd, DBL, ROOT, COM);

  if (errno)
    goto end;

  if (id == ROOT) {
    vec_fput(stdout, fp);
    putchar('\n');
  }
end:
  if (t)
    free(t);

  if (td)
    free(td);

  if (tc)
    free(tc);

  if (vd)
    free(vd);

  if (vc)
    free(vc);

  if (fd)
    free(fd);

  if (xp)
    free(xp);

  if (fp)
    free(fp);

  if (mp)
    free(mp);

  if (errno) {
    MPI_Abort(COM, errno);
    printf("fatal: %s.\n", strerror(errno));
    return -1;
  }

  if ((errno = MPI_Finalize())) {
    printf("fatal: %s.\n", strerror(errno));
    MPI_Abort(COM, errno);
    return -1;
  }

  return 0;
}
