#include <gmp_als.hpp>

#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <queue>
#include <vector>

using namespace std;

int gmp_als::fget(ifstream& f) {
  int a, b, ec;

  f >> a >> ec;
  al.resize(a);

  for (int i = 0; i < ec; ++i) {
    if (f.eof() || f.bad())
      return -1;

    f >> a >> b;

    al[a - 1].push_back(b - 1);
    al[b - 1].push_back(a - 1);
  }

  return 0;
}

int gmp_als::fput(ofstream& f) {
  for (size_t i = 0; i < al.size(); ++i)
    vput(f, i);

  return 0;
}

int gmp_als::vput(ofstream& f, int v) {
  sort(al[v].begin(), al[v].end(), less<int>());

  f << v + 1 << ": ";

  for (int s : al[v])
    f << s + 1 << " ";

  f << endl;

  return 0;
}

int gmp_als::bfs(int sp, function<bool(int, int)> f) {
  vector<bool> chk(al.size(), false);
  queue<pair<int, int>> que;

  que.push(pair(sp, 0));
  chk[sp] = true;

  while (!que.empty()) {
    pair v = que.front();

    if (!f(v.first, v.second))
      break;

    for (int c : al[v.first])
      if (!chk[c]) {
        que.push(pair(c, v.second + 1));
        chk[c] = true;
      }

    que.pop();
  }

  return 0;
}

int gmp_als::spp(int sp, int ep, int& r) {
  r = -1;

  bfs(sp, [&ep, &r](int v, int lvl) {
    if (v == ep) {
      r = lvl;
      return false;
    }

    return true;
  });

  return 0;
}
