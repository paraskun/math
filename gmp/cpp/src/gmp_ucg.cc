#include <cerrno>
#include <cstring>
#include <functional>
#include <queue>
#include <vector>

#include <gmp_ucg.hpp>

using namespace std;

int gmp_ucg::add(int a, int b) {
  if (a == b)
    return 0;

  list<int>::iterator i = al[a].begin();

  while (i != al[a].end() && *i < b)
    advance(i, 1);

  al[a].insert(i, b);

  return 0;
}

int gmp_ucg::add(gmp_ucg& g) {
  if (al.size() < g.al.size())
    al.resize(g.al.size());

  for (size_t i = 0; i < al.size(); ++i) {
    al[i].merge(g.al[i]);
    al[i].unique();
  }

  return 0;
}

int gmp_ucg::fget_edg(istream& f) {
  int a, b, c;

  f >> a >> c;
  al.resize(a);

  for (int i = 0; i < c; ++i) {
    if (f.eof() || f.bad()) {
      errno = EIO;
      return -1;
    }

    f >> a >> b;

    add(a - 1, b - 1);
    add(b - 1, a - 1);
  }

  return 0;
}

int gmp_ucg::fget_vtx(istream& f) {
  list<int> prev;
  int a, c;

  f >> c;

  for (int i = 0; i < c; ++i) {
    if (f.eof() || f.bad()) {
      errno = EIO;
      return -1;
    }

    f >> a;

    if ((size_t)a > al.size())
      al.resize(a);

    for (const int& v : prev) {
      add(a - 1, v - 1);
      add(v - 1, a - 1);
    }

    prev.push_back(a);
  }

  return 0;
}

int gmp_ucg::fput(ostream& f) {
  for (size_t i = 0; i < al.size(); ++i)
    vput(f, i);

  return 0;
}

int gmp_ucg::vput(ostream& f, int v) {
  if (f.bad()) {
    errno = EIO;
    return -1;
  }

  f << v + 1 << ": ";

  for (const int& s : al[v])
    if (!f.bad())
      f << s + 1 << " ";
    else {
      errno = EIO;
      return -1;
    }

  f << endl;

  return 0;
}

int gmp_ucg::bfs(int sp, function<bool(int, int)> f) {
  vector<bool> check(al.size(), false);
  queue<pair<int, int>> que;

  que.push(pair{sp, sp});
  check[sp] = true;

  while (!que.empty()) {
    pair vtx = que.front();

    if (!f(vtx.first, vtx.second))
      break;

    for (const int& c : al[vtx.first])
      if (!check[c]) {
        que.push(pair{c, vtx.first});
        check[c] = true;
      }

    que.pop();
  }

  return 0;
}

int gmp_ucg::spp(int sp, int ep, list<int>& path) {
  vector<int> ft(al.size(), -1);

  bfs(sp, [&ep, &ft](int v, int p) {
    ft[v] = p;

    if (v == ep)
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

void gmp_ucg::clear() {
  al.clear();
}
