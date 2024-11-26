#include <fem/const.h>
#include <fem/hex.h>
#include <mtx/mtx.h>

#include <stdlib.h>

struct hex* hex_new() {
  struct hex* h = malloc(sizeof(struct hex));

  if (!h)
    return h;

  h->dep.m = mtx_new(8);
  h->dep.g = mtx_new(8);
  h->dep.b = vec_new(8);

  return h;
}

int hex_get(FILE* obj, struct hex* h) {
  if (fgetc(obj) != 'h')
    return 0;

  for (int j = 0, v; j < 8; ++j) {
    fscanf(obj, "%d", &v);
    h->vtx[j] = v - 1;
  }

  fscanf(obj, "| %lf | %lf", &h->pps.lam, &h->pps.gam);

  return 1;
}

int hex_evo(struct hex* h, struct vtx** v) {
  double lam = h->pps.lam;
  double gam = h->pps.gam;

  double hx = v[h->vtx[1]]->x - v[h->vtx[0]]->x;
  double hy = v[h->vtx[2]]->y - v[h->vtx[0]]->y;
  double hz = v[h->vtx[4]]->z - v[h->vtx[0]]->z;

  double gx[2][2];
  double gy[2][2];
  double gz[2][2];

  double mx[2][2];
  double my[2][2];
  double mz[2][2];

  struct mtx* d = mtx_new(8);
  struct vec* q = vec_new(8);

  double X[2][2] = {{v[h->vtx[0]]->x / hx + 0.5, -v[h->vtx[0]]->x / hx - 0.5},
                    {-1, 1}};

  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 2; ++j) {
      gx[i][j] = G[i][j] / hx;
      gy[i][j] = G[i][j] / hy;
      gz[i][j] = G[i][j] / hz;

      mx[i][j] = M[i][j] * hx;
      my[i][j] = M[i][j] * hy;
      mz[i][j] = M[i][j] * hz;
    }

  double** gv = h->dep.g->v;
  double** mv = h->dep.m->v;

  double** dv = d->v;
  double* qv = q->vp;

  for (int i = 0, ib = 0; i < 8; ++i, ib += 8) {
    qv[i] = v[h->vtx[i]]->pps.q;

    int mui = MU[i];
    int nui = NU[i];
    int tti = TT[i];

    for (int j = 0; j < 8; ++j) {
      int muj = MU[j];
      int nuj = NU[j];
      int ttj = TT[j];

      double mxl = mx[mui][muj];
      double myl = my[nui][nuj];
      double mzl = mz[tti][ttj];

      double gxl = gx[mui][muj];
      double gyl = gy[mui][muj];
      double gzl = gz[mui][muj];

      mv[i][j] = gam * mxl * myl * mzl;
      gv[i][j] = lam * (gxl * myl * mzl + mxl * gyl * mzl + mxl * myl * gzl);
      dv[i][j] = X[mui][muj] * myl * mzl;
    }
  }

  mtx_vmlt(d, q, h->dep.b);

  mtx_cls(d);
  vec_cls(q);

  return 0;
}

int hex_cls(struct hex* h) {
  if (!h)
    return 0;

  mtx_cls(h->dep.g);
  mtx_cls(h->dep.m);
  vec_cls(h->dep.b);

  free(h);

  return 0;
}
