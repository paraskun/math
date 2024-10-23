#include <gmp_ucg.hpp>

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <fstream>
#include <functional>
#include <queue>
#include <vector>

using namespace std;

int gmp_ucg::fget(ifstream& f) {
  int a, b, ec;

  f >> a >> ec;
  al.resize(a);

  for (int i = 0; i < ec; ++i) {
    if (f.eof() || f.bad()) {
      errno = EIO;
      return -1;
    }

    f >> a >> b;

    al[a - 1].push_back(b - 1);
    al[b - 1].push_back(a - 1);
  }

  return 0;
}

int gmp_ucg::fput(ofstream& f) {
  for (size_t i = 0; i < al.size(); ++i)
    vput(f, i);

  return 0;
}

int gmp_ucg::vput(ofstream& f, int v) {
  if (f.bad()) {
    errno = EIO;
    return -1;
  }

  sort(al[v].begin(), al[v].end(), less<int>());

  f << v + 1 << ": ";

  for (int s : al[v])
    if (!f.bad())
      f << s + 1 << " ";
    else {
      errno = EIO;
      return -1;
    }

  f << endl;

  return 0;
}

int gmp_ucg::bfs(int sp, function<bool(gmp_vtx)> f) {
  vector<bool> chk(al.size(), false);
  queue<gmp_vtx> que;

  que.push(gmp_vtx{sp, sp});
  chk[sp] = true;

  while (!que.empty()) {
    gmp_vtx vtx = que.front();

    if (!f(vtx))
      break;

    for (int c : al[vtx.v])
      if (!chk[c]) {
        que.push(gmp_vtx{c, vtx.v});
        chk[c] = true;
      }

    que.pop();
  }

  return 0;
}

int gmp_ucg::spp(int sp, int ep, list<int>& path) {
  vector<int> ft(al.size(), -1);

  bfs(sp, [&ep, &ft](gmp_vtx vtx) {
    ft[vtx.v] = vtx.p;

    if (vtx.v == ep)
      return false;

    return true;
  });

  if (ft[ep] == -1)
    return 0;

  int v = ep;
  path.push_front(v + 1);

  do {
    v = ft[v];
    path.push_front(v + 1);
  } while (ft[v] != v);

  return 0;
}
