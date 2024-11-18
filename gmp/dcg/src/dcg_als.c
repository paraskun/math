#include <dcg_als.h>

#include <errno.h>
#include <limits.h>
#include <stdlib.h>

struct dcg_sll* dcg_sll_new() {
  struct dcg_sll* l = malloc(sizeof(struct dcg_sll));

  if (!l)
    return NULL;

  l->s = 0;
  l->beg = NULL;
  l->end = NULL;

  return l;
}

int dcg_sll_add(struct dcg_sll* l, int vtx, int wgt) {
  if (!l) {
    errno = EINVAL;
    return -1;
  }

  struct dcg_edg* e = NULL;
  struct dcg_edg* n = l->beg;

  while (n && n->vtx < vtx) {
    e = n;
    n = n->next;
  }

  return dcg_sll_ins(l, &e, vtx, wgt);
}

int dcg_sll_ins(struct dcg_sll* l, struct dcg_edg** e, int vtx, int wgt) {
  if (!l) {
    errno = EINVAL;
    return -1;
  }

  struct dcg_edg* n = malloc(sizeof(struct dcg_edg));
  struct dcg_edg* p = *e;

  if (!n)
    return -1;

  n->vtx = vtx;
  n->wgt = wgt;

  if (p) {
    n->next = p->next;
    p->next = n;

    if (p == l->end)
      l->end = n;
  } else {
    n->next = l->beg;
    l->beg = n;

    if (!l->s)
      l->end = n;
  }

  *e = n;
  l->s += 1;

  return 0;
}

int dcg_sll_srh(struct dcg_sll* l, struct dcg_edg** e, int vtx) {
  if (!l || vtx < 0) {
    errno = EINVAL;
    return -1;
  }

  struct dcg_edg* n = l->beg;

  while (n && n->vtx < vtx)
    n = n->next;

  *e = NULL;

  if (n && n->vtx == vtx)
    *e = n;

  return 0;
}

int dcg_sll_cls(struct dcg_sll* l) {
  if (!l)
    return -1;

  struct dcg_edg* e = l->beg;
  struct dcg_edg* n;

  while (e) {
    n = e;
    e = e->next;

    free(n);
  }

  return 0;
}

struct dcg_als* dcg_als_new(int s) {
  struct dcg_als* g = malloc(sizeof(struct dcg_als));

  if (!g)
    return NULL;

  g->s = s;
  g->als = malloc(sizeof(struct dcg_sll*) * s);

  for (int i = 0; i < s; ++i)
    g->als[i] = dcg_sll_new();

  return g;
}

int dcg_als_get(FILE* f, struct dcg_als* g, int w) {
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
    int a, b, c;

    if (fscanf(f, "%d %d", &a, &b) != 2)
      return -1;

    if (w) {
      if (fscanf(f, "%d", &c) != 1)
        return -1;
    } else
      c = rand();

    if (dcg_als_add(g, a - 1, b - 1, c))
      return -1;
  }

  return 0;
}

int dcg_als_map(FILE* f, struct dcg_als* g) {
  if (!f || !g || !g->map) {
    errno = EINVAL;
    return -1;
  }

  int s = g->s;

  for (int i = 0; i < s; ++i) {
    struct dcg_edg* e = g->als[i]->beg;

    while (e) {
      int t = e->vtx;
      int n = g->map[i][t];

      fprintf(f, "%d -> %d (%d): %d - ", i + 1, t + 1, e->wgt, i + 1);

      while (n != t) {
        fprintf(f, "%d - ", n + 1);
        n = g->map[n][t];
      }

      fprintf(f, "%d\n", e->vtx + 1);

      e = e->next;
    }
  }

  return 0;
}

int dcg_als_add(struct dcg_als* g, int f, int t, int wgt) {
  if (!g)
    return -1;

  if (f < 0 || f >= g->s || t < 0 || t >= g->s) {
    errno = EINVAL;
    return -1;
  }

  if (dcg_sll_add(g->als[f], t, wgt))
    return -1;

  return 0;
}

int dcg_als_fwp(struct dcg_als* g, int map) {
  int s = g->s;

  if (map) {
    g->map = malloc(sizeof(int*) * s);

    if (!g->map)
      return -1;

    for (int i = 0; i < s; ++i) {
      g->map[i] = malloc(sizeof(int) * s);

      if (!g->map[i]) {
        for (int j = 0; j < i; ++j)
          free(g->map[j]);

        free(g->map);
        g->map = NULL;

        return -1;
      }
    }

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
    for (int u = 0; u < s; ++u) {
      struct dcg_edg* ue = g->als[u]->beg;

      while (ue) {
        g->map[u][ue->vtx] = ue->vtx;
        ue = ue->next;
      }
    }
  }

  for (int i = 0; i < s; ++i) {
    struct dcg_sll* il = g->als[i];

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
    for (int u = 0; u < s; ++u) {
      if (i == u)
        continue;

      struct dcg_sll* ul = g->als[u];

      struct dcg_edg* ie;
      struct dcg_edg* ue;
      struct dcg_edg* pe;

      dcg_sll_srh(ul, &ie, i);

      if (!ie)
        continue;

      int uiw = ie->wgt;

      ie = il->beg;
      ue = ul->beg;
      pe = NULL;

      while (ie) {
        int v = ie->vtx;
        int ivw = ie->wgt;

        if (!ue || v < ue->vtx) {
          dcg_sll_ins(ul, &pe, v, uiw + ivw);

          if (map)
            g->map[u][v] = g->map[u][i];

          ie = ie->next;
          continue;
        }

        if (v > ue->vtx) {
          pe = ue;
          ue = ue->next;
          continue;
        }

        if (uiw + ivw < ue->wgt) {
          ue->wgt = uiw + ivw;

          if (map)
            g->map[u][v] = g->map[u][i];
        }

        pe = ue;
        ue = ue->next;
        ie = ie->next;
      }
    }
  }

  return 0;
}

int dcg_als_cls(struct dcg_als* g) {
  if (!g)
    return -1;

  int s = g->s;

  for (int i = 0; i < s; ++i)
    if (dcg_sll_cls(g->als[i]))
      return -1;

  if (g->map) {
    for (int i = 0; i < s; ++i)
      free(g->map[i]);

    free(g->map);
  }

  free(g);
  return 0;
}
