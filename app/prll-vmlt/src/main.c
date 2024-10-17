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

#define DIN "dms.in"
#define XIN "x.in"
#define MIN "pmtx.in"

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

  int n;

  struct vec* x = 0;
  struct vec* f = 0;
  struct pmtx* m = 0;

  MPI_Request* req = 0;

  FILE* fd = fopen(DIN, "r");

  if (!fd)
    goto err;

  if (fscanf(fd, "%d", &n) != 1) {
    fclose(fd);
    goto err;
  }

  fclose(fd);

  if (pc > n)
    pc = n;

  if (id >= pc) {
    MPI_Finalize();
    return 0;
  }

  if (!(x = vec_new(n)))
    goto err;

  if (!(f = vec_new(n)))
    goto err;

  FILE* fx = fopen(XIN, "r");

  if (!fx)
    goto err;

  if (vec_fget(fx, x)) {
    fclose(fx);
    goto err;
  }

  fclose(fx);

  if (id == ROOT && pc > 1) {
    req = malloc(sizeof(MPI_Request) * (pc - 1));

    if (!req)
      goto err;
  }

  int c = n;
  int s = n;

  for (int i = pc - 1; i >= 0; --i) {
    c = s / (i + 1);
    s -= c;

    if (i != ROOT && id == ROOT)
      if ((errno = MPI_Irecv(f->v + s, c, DBL, i, TAG, COM, &req[i - 1])))
        goto err;

    if (id == i)
      break;
  }

  if (!(m = pmtx_new(n, s, c)))
    goto err;

  FILE* fm = fopen(MIN, "r");

  if (!fm)
    goto err;

  if (mtx_fget(fm, m)) {
    fclose(fm);
    goto err;
  }

  fclose(fm);

  if (mtx_vmlt(m, x, f))
    goto err;

  if (id != ROOT) {
    if ((errno = MPI_Send(f->v + s, c, DBL, ROOT, TAG, COM)))
      goto err;
  } else {
    if ((errno = MPI_Waitall(pc - 1, req, NULL)))
      goto err;

    if (vec_fput(stdout, f))
      goto err;

    putchar('\n');
  }

  if (req)
    free(req);

  vec_free(x);
  vec_free(f);
  mtx_free(m);

  if ((errno = MPI_Finalize())) {
    printf("fatal: %s.\n", strerror(errno));
    MPI_Abort(COM, errno);
    return -1;
  }

  return 0;

err:
  if (req)
    free(req);

  if (x)
    vec_free(x);

  if (f)
    vec_free(f);

  if (m)
    mtx_free(m);

  MPI_Abort(COM, errno);
  printf("fatal: %s.\n", strerror(errno));

  return -1;
}
