#include <dcg_als.h>

#include <errno.h>
#include <limits.h>
#include <stdlib.h>

struct list* list_new() {
  struct list* l = malloc(sizeof(struct list));

  if (!l)
    return NULL;

  l->n = 0;
  l->top = NULL;
  l->bot = NULL;

  return l;
}

int list_add(struct list* l, int vtx, int wgt) {
  struct edge* e = malloc(sizeof(struct edge));

  if (!e)
    return -1;

  e->vtx = vtx;
  e->wgt = wgt;
  e->next = NULL;

  if (!l->top) {
    l->bot = e;
    l->top = e;
  } else {
    l->bot->next = e;
    l->bot = e;
  }

  return 0;
}

int list_cls(struct list* l) {
  if (!l)
    return -1;

  struct edge* e = l->top;
  struct edge* n;

  while (e) {
    n = e;
    e = e->next;

    free(n);
  }

  return 0;
}

struct dcg_als* dcg_als_new(int n) {
  struct dcg_als* g = malloc(sizeof(struct dcg_als));

  if (!g)
    return NULL;

  g->n = n;
  g->als = malloc(sizeof(struct list*) * n);

  for (int i = 0; i < n; ++i)
    g->als[i] = list_new();

  return g;
}

int dcg_als_get(FILE* f, struct dcg_als* g) {
  if (!f || !g) {
    errno = EINVAL;
    return -1;
  }

  int v;

  if (fscanf(f, "%d", &v) != 1)
    return -1;

  if (v < 1) {
    errno = EINVAL;
    return -1;
  }

  for (int i = 0; i < v; ++i) {
    int a, b;
    int w = rand();

    if (fscanf(f, "%d %d", &a, &b) != 2)
      return -1;

    if (dcg_als_add(g, a - 1, b - 1, w))
      return -1;
  }

  return 0;
}

int dcg_als_map(FILE* f, int n, int** len, int** map) {
  if (!f || !len || !map) {
    errno = EINVAL;
    return -1;
  }

  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j) {
      fprintf(f, "%d -> %d: ", i + 1, j + 1);

      if (len[i][j] == INT_MAX) {
        fprintf(f, "nope.\n");
        continue;
      }

      fprintf(f, "%d - ", i + 1);

      int n = map[i][j];

      while (n != j) {
        fprintf(f, "%d - ", n + 1);
        n = map[n][j];
      }

      fprintf(f, "%d\n", j + 1);
    }

  return 0;
}

int dcg_als_add(struct dcg_als* g, int a, int b, int wgt) {
  if (!g)
    return -1;

  if (a < 0 || a >= g->n || b < 0 || b >= g->n) {
    errno = EINVAL;
    return -1;
  }

  if (list_add(g->als[a], b, wgt))
    return -1;

  return 0;
}

int dcg_als_fwp(struct dcg_als* g, int** len, int** map) {
  int n = g->n;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < n; ++i) {
    int* li = len[i];
    int* mi = map[i];

    for (int j = 0; j < n; ++j)
      li[j] = INT_MAX;

    struct edge* e = g->als[i]->top;

    while (e) {
      li[e->vtx] = e->wgt;

      if (map)
        mi[e->vtx] = e->vtx;

      e = e->next;
    }
  }

  for (int i = 0; i < n; ++i) {
    int* li = len[i];

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
    for (int u = 0; u < n; ++u) {
      int* lu = len[u];
      int* mu = map[u];

      if (lu[i] == INT_MAX)
        continue;

      int ui = lu[i];

      for (int v = 0; v < n; ++v) {
        if (li[v] == INT_MAX)
          continue;

        int iv = li[v];
        int uv = ui + iv;

        if (uv < lu[v]) {
          lu[v] = uv;

          if (map)
            mu[v] = mu[i];
        }
      }
    }
  }

  return 0;
}

int dcg_als_cls(struct dcg_als* g) {
  if (!g)
    return -1;

  for (int i = 0; i < g->n; ++i)
    if (list_cls(g->als[i]))
      return -1;

  free(g);

  return 0;
}
