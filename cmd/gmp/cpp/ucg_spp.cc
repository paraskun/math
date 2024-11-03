#include <gmp_ucg.hpp>

#include <cerrno>
#include <cstring>
#include <fstream>
#include <iostream>
#include <list>

using namespace std;

int main(int argc, char** argv) {
  gmp_ucg g;

  std::ifstream in;
  std::ofstream out;

  list<int> path;

  int sp, ep;

  if (argc < 4) {
    errno = EINVAL;
    goto end;
  }

  sp = strtod(argv[2], 0);
  ep = strtod(argv[3], 0);

  if (sp == 0 || ep == 0) {
    errno = EINVAL;
    goto end;
  }

  in.open(argv[1]);

  if (!in.is_open()) {
    errno = EIO;
    goto end;
  }

  if (g.fget_edg(in))
    goto end;

  out.open("gmpcc-out.txt");

  if (!out.is_open()) {
    errno = EIO;
    goto end;
  }

  g.bfs(sp - 1, [&g, &out](int v, int) {
    g.vput(out, v);
    return true;
  });

  g.spp(sp - 1, ep - 1, path);

  if (path.size() != 0) {
    cout << "Shortest path: ";

    for (int v : path)
      cout << v << " ";

    cout << endl;
  } else
    cout << "Path does not exists." << endl;

end:
  if (in.is_open())
    in.close();

  if (out.is_open())
    out.close();

  if (errno) {
    cout << "fatal: " << strerror(errno) << endl;
    return -1;
  }

  return 0;
}
