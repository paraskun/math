#include <bgs/dcg.h>

#include <gds/hset.h>
#include <gds/pque.h>

#include <errno.h>
#include <stdlib.h>

int dcg_new(struct dcg** h, unsigned cap) {
  if (!h) {
    errno = EINVAL;
    return -1;
  }

  struct dcg* g = malloc(sizeof(struct dcg));

  if (!g) {
    errno = ENOMEM;
    return -1;
  }

  g->cap = cap;
  g->data = malloc(sizeof(struct pssll*));

  if (!g) {
    free(g);

    errno = ENOMEM;
    return -1;
  }

  for (unsigned i = 0; i < cap; ++i)
    if (ssll_new(&g->data[i])) {
      for (unsigned j = 0; j < i; ++j)
        ssll_cls(g->data[j]);

      free(g->data);
      free(g);

      errno = ENOMEM;
      return -1;
    }

  *h = g;

  return 0;
}

int dcg_add(struct dcg* g, unsigned src, unsigned dst, int wgt) {
  if (!g || src >= g->cap || dst >= g->cap) {
    errno = EINVAL;
    return -1;
  }

  struct edge* e = malloc(sizeof(struct edge));

  if (!e) {
    errno = ENOMEM;
    return -1;
  }

  e->vtx = dst;
  e->wgt = wgt;

  if (ssll_add(g->data[src], e)) {
    free(e);
    return -1;
  }

  return 0;
}

static int cmp(void* a, void* b) {
  struct edge* ea = (struct edge*)a;
  struct edge* eb = (struct edge*)b;

  if (ea->wgt < eb->wgt)
    return 1;

  if (ea->wgt > eb->wgt)
    return -1;

  return 0;
}

int dcg_ssp(struct dcg* g, unsigned src, struct edge* map) {
  if (!g || !map) {
    errno = EINVAL;
    return -1;
  }

  struct ppque* que = NULL;
  struct ihset* set = NULL;

  pque_new(&que, g->cap);
  hset_new(&set, g->cap);

  unsigned* ind = malloc(sizeof(int) * (g->cap + 1));

  if (!ind) {
    pque_cls(que);
    hset_cls(set);

    errno = ENOMEM;
    return -1;
  }

  que->len = g->cap;
  que->cmp = &cmp;
  que->ind = ind;

  for (unsigned i = 0, p = 2; i < g->cap; ++i)
    if (i != src) {
      map[i].vtx = 0;
      map[i].wgt = -1;
      
      que->data[p] = &map[i];
      ind[p] = i;

      p += 1;
    } else {
      map[i].vtx = src;
      map[i].wgt = 0;

      que->data[1] = &map[i];
      ind[1] = i;
    }

  struct edge* e = NULL;

  while (que->len != 0) {
    pque_ext(que, (void**)&e);
    hset_ins(set, ind[1]);

    struct pslln* n = g->data[ind[1]]->beg;

    while (n) {
      struct edge* v = (struct edge*)n->e;

      if (!hset_has(set, v->vtx)) {
        int alt = map[ind[1]].wgt + v->wgt;

        if (alt < map[v->vtx].wgt) {
          map[v->vtx].wgt = alt;

          for (unsigned i = 1; i <= g->cap; ++i) {
            if (ind[i] == v->vtx) {
              pque_fixu(que, i);
              break;
            }
          }
        }
      }
       
      n = n->next;
    }
  }

  free(ind);

  return 0;
}

int dcg_asp(struct dcg* g, struct edge** map) {
  if (!g || !map) {
    errno = EINVAL;
    return -1;
  }

  for (unsigned i = 0; i < g->cap; ++i)
    dcg_ssp(g, i, map[i]);

  return 0;
}

int dcg_cls(struct dcg* g) {
  if (!g) {
    errno = EINVAL;
    return -1;
  }

  for (unsigned i = 0; i < g->cap; ++i) {
    struct pslln* n = g->data[i]->beg;

    while (n) {
      free(n->e);

      n = n->next;
    }

    ssll_cls(g->data[i]);
  }

  free(g->data);
  free(g);

  return 0;
}
