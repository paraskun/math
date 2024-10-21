#include <errno.h>
#include <mpi.h>
#include <stdarg.h>
#include <time.h>
#include <vec.h>

#define N 100000

#define TAG 0
#define DBL MPI_DOUBLE
#define COM MPI_COMM_WORLD
#define ANY MPI_ANY_SOURCE
#define ROOT 0

void trace(const char* fmt, ...) {
  time_t t = time(0);
  char tb[25];
  va_list args;

  strftime(tb, 25, "%F %T", localtime(&t));
  printf("[%s]: ", tb);

  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);

  putchar('\n');
}

int main(int argc, char* argv[argc]) {
  if ((errno = MPI_Init(&argc, &argv)))
    goto err;

  int pc;
  int id;

  if ((errno = MPI_Comm_size(COM, &pc)))
    goto err;

  if ((errno = MPI_Comm_rank(COM, &id)))
    goto err;

  double nrm;

  if (id == ROOT)
    for (int i = 0; i < pc - 1; ++i) {
      MPI_Status stat;

      if ((errno = MPI_Recv(&nrm, 1, DBL, ANY, TAG, COM, &stat)))
        goto err;

      trace("%d -> %lf", stat.MPI_SOURCE, nrm);
    }
  else {
    struct vec* v = vec_new(N);

    if (!v)
      goto err;

    vec_seq(v, id);
    vec_nrm(v, &nrm);

    if ((errno = MPI_Send(&nrm, 1, DBL, ROOT, TAG, COM))) {
      vec_free(v);
      goto err;
    }

    vec_free(v);
  }

  if ((errno = MPI_Finalize()))
    goto err;

  return 0;

err:
  MPI_Abort(COM, errno);
  return -1;
}
