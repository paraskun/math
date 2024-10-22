#include <gmp_als.hpp>

#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <queue>
#include <vector>

using namespace std;

int gmp_als::fget(ifstream& f) {
  if (!f.is_open())
    return -1;

  int a, b, ec;

  f >> a >> ec;
  al.resize(a);

  for (int i = 0; i < ec; ++i) {
    if (f.eof() || f.bad())
      return -1;

    f >> a >> b;
    al[a].push_back(b);
  }

  return 0;
}

int gmp_als::fput(ofstream& f) {
  for (size_t i = 0; i < al.size(); ++i) {
    sort(al[i].begin(), al[i].end(), less<int>());

    f << i + 1 << ": ";

    for (int v : al[i])
      f << v << " ";

    f << endl;
  }

  return 0;
}

int gmp_als::bfs(int sp, function<bool(int, int)> f) {
  vector<bool> chk(al.size(), false);
  queue<pair<int, int>> que;

  que.push(pair(sp, 0));

  while (!que.empty()) {
    pair v = que.front();

    chk[v.first] = true;

    if (f && !f(v.first, v.second))
      break;

    for (int c : al[v.first])
      if (!chk[c])
        que.push(pair(c, v.second + 1));

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
