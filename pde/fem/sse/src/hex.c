#include <fem/hex.h>
#include <fem/const.h>
#include <mtx_all.h>

#include <stdlib.h>

struct hex* hex_new() {
  struct hex* h = malloc(sizeof(struct hex));

  if (!h)
    return h;

  h->g = mtx_new(8);
  h->m = mtx_new(8);
  h->b = vec_new(8);
  h->q = vec_new(8);

  h->fll.beg = NULL;
  h->fll.end = NULL;

  return h;
}

int hex_get(FILE* obj, struct hex* h) {
  for (int j = 0, v; j < 8; ++j) {
    fscanf(obj, "%d", &v);
    h->vtx[j] = v - 1;
  }

  fscanf(obj, "| %lf | %lf", &h->lam, &h->gam);

  return 1;
}

int hex_evo(struct hex* h, struct vtx** v, struct fll* l) {
  double lam = h->lam;
  double gam = h->gam;

  double hx = v[h->vtx[1]]->x - v[h->vtx[0]]->x;
  double hy = v[h->vtx[2]]->y - v[h->vtx[0]]->y;
  double hz = v[h->vtx[4]]->z - v[h->vtx[0]]->z;

  double Gx[2][2];
  double Gy[2][2];
  double Gz[2][2];

  double Mx[2][2];
  double My[2][2];
  double Mz[2][2];

  struct mtx* D = mtx_new(8);

  double X[2][2] = {
    {v[h->vtx[0]]->x / hx + 0.5, - v[h->vtx[0]]->x / hx - 0.5},
    {-1, 1}
  };

  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 2; ++j) {
      Gx[i][j] = G[i][j] / hx;
      Gy[i][j] = G[i][j] / hy;
      Gz[i][j] = G[i][j] / hz;

      Mx[i][j] = M[i][j] * hx;
      My[i][j] = M[i][j] * hy;
      Mz[i][j] = M[i][j] * hz;
    }

  double* gvp = h->g->vp;
  double* mvp = h->m->vp;
  double* dvp = D->vp;

  for (int i = 0, ib = 0; i < 8; ++i, ib += 8) {
    int mui = MU[i];
    int nui = NU[i];
    int tti = TT[i];

    for (int j = 0; j < 8; ++j) {
      int e = ib + j;

      int muj = MU[j];
      int nuj = NU[j];
      int ttj = TT[j];

      double mx = Mx[mui][muj];
      double my = My[nui][nuj];
      double mz = Mz[tti][ttj];

      double gx = Gx[mui][muj];
      double gy = Gy[mui][muj];
      double gz = Gz[mui][muj];

      mvp[e] = gam * mx * my * mz;
      dvp[e] = X[mui][muj] * my * mz;
      gvp[e] = lam * (gx * my * mz + mx * gy * mz + mx * my * gz);
    }
  }

  mtx_vmlt(D, h->q, h->b);

  struct fce* f = h->fll.beg;

  while (f) {
    switch (f->cnd.type) {
      case DIR:
        fll_add(l, f);
        break;
      case NEU:
      case ROB:
        fce_evo(f, v, hx, hy, hz, Mx, My, Mz);
    }

    f = f->next;
  }

  return 0;
}

int hex_cls(struct hex* h) {
  if (!h)
    return 0;

  mtx_cls(h->g);
  mtx_cls(h->m);
  vec_cls(h->b);
  vec_cls(h->q);
  fll_cls(&h->fll);

  free(h);

  return 0;
}
