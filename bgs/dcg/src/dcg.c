#include <bgs/dcg.h>

#include <dsa/hset.h>
#include <dsa/pque.h>

#include <errno.h>
#include <stdlib.h>

int dcg_new(struct dcg* g, uint cap) {
  if (!g) {
    errno = EINVAL;
    return -1;
  }

  g->cap = cap;
  g->data = malloc(sizeof(struct psll));

  if (!g) {
    errno = ENOMEM;
    return -1;
  }

  for (uint i = 0; i < cap; ++i)
    if (sll_new(&g->data[i])) {
      for (uint j = 0; j < i; ++j)
        sll_cls(&g->data[j]);

      free(g->data);

      errno = ENOMEM;
      return -1;
    }

  return 0;
}

int dcg_add(struct dcg* g, uint src, uint dst, uint wgt) {
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

  if (sll_add(&g->data[src], e)) {
    free(e);
    return -1;
  }

  return 0;
}

static int cmp_wgt(void* a, void* b) {
  struct path* pa = (struct path*)a;
  struct path* pb = (struct path*)b;

  if (pa->wgt < pb->wgt)
    return 1;

  if (pa->wgt > pb->wgt)
    return -1;

  return 0;
}

static int cmp_dst(void* a, void* b) {
  struct path* pa = (struct path*)a;
  struct path* pb = (struct path*)b;

  if (pa->dst > pb->dst)
    return 1;

  if (pa->dst < pb->dst)
    return -1;

  return 0;
}

int dcg_ssp(struct dcg* g, uint src, struct path** map) {
  if (!g || !map) {
    errno = EINVAL;
    return -1;
  }

  for (uint i = 0; i < g->cap; ++i)
    if (i == src) {
      map[i]->dst = i;
      map[i]->hop = i;
      map[i]->wgt = 0;
    } else {
      map[i]->dst = i;
      map[i]->hop = 0;
      map[i]->wgt = INF;
    }

  struct ppque que;
  struct ihset set;

  que.cap = g->cap;
  que.len = g->cap;
  que.cmp = &cmp_wgt;
  que.data = (void**)map;

  if (pque_fix(&que))
    return -1;

  if (hset_new(&set, g->cap))
    return -1;

  struct path* p = NULL;

  while (que.len != 0) {
    pque_ext(&que, (void**)&p);
    hset_ins(&set, p->dst);

    struct pslln* n = g->data[p->dst].beg;

    while (n) {
      struct edge* v = (struct edge*)n->e;

      if (!hset_has(&set, v->vtx)) {
        uint alt = p->wgt + v->wgt;
        uint wgt = map[v->vtx]->wgt;

        if (wgt == INF || alt < wgt) {
          map[v->vtx]->wgt = alt;
          map[v->vtx]->hop = p->dst;

          for (uint i = 1; i <= que.len; ++i) {
            if (((struct path*)que.data[i])->dst == v->vtx) {
              pque_fixu(&que, i);
              break;
            }
          }
        }
      }

      n = n->next;
    }
  }

  que.len = que.cap;
  que.cmp = &cmp_dst;

  pque_fix(&que);
  pque_srt(&que);
  hset_cls(&set);

  return 0;
}

int dcg_asp(struct dcg* g, struct path*** map) {
  if (!g || !map) {
    errno = EINVAL;
    return -1;
  }

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
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
    struct pslln* n = g->data[i].beg;

    while (n) {
      free(n->e);
      n = n->next;
    }

    sll_cls(&g->data[i]);
  }

  free(g->data);

  return 0;
}
