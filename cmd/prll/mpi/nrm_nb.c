#include <errno.h>
#include <mpi.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <vec.h>

#define N 100000

#define TAG 0
#define DBL MPI_DOUBLE
#define COM MPI_COMM_WORLD
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

  if (id == ROOT) {
    MPI_Request* req = malloc(sizeof(MPI_Request) * (pc - 1));

    if (!req)
      goto err;

    double* nrm = malloc(sizeof(double) * (pc - 1));

    if (!nrm) {
      free(req);
      goto err;
    }

    int* f = malloc(sizeof(int) * (pc - 1));

    if (!f) {
      free(req);
      free(nrm);
      goto err;
    }

    for (int i = 0; i < pc - 1; ++i) {
      f[i] = 0;

      if ((errno = MPI_Irecv(&nrm[i], 1, DBL, i + 1, TAG, COM, &req[i]))) {
        free(req);
        free(nrm);
        free(f);

        goto err;
      }
    }

    int s = 0;

    while (s != pc - 1)
      for (int i = 0; i < pc - 1; ++i)
        if (!f[i]) {
          if ((errno = MPI_Test(&req[i], &f[i], NULL))) {
            free(req);
            free(nrm);
            free(f);

            goto err;
          }

          if (f[i]) {
            s += 1;
            trace("%d -> %lf", i + 1, nrm[i]);
          }
        }

    free(req);
    free(nrm);
    free(f);
  } else {
    srand(time(NULL) + id);

    struct vec* v = vec_new(N);

    if (!v)
      goto err;

    double nrm;

    vec_seq(v, id);
    vec_nrm(v, &nrm);

    if (id == 1)
      sleep(10);

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
