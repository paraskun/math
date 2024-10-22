#include <gmp_als.hpp>

#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
  if (argc < 3)
    return -1;

  int sp = strtod(argv[1], 0);
  int ep = strtod(argv[2], 0);

  struct gmp_als g;

  std::ifstream in;
  std::ofstream out;

  in.open("gmpcc-in.txt");

  if (!in.is_open())
    goto end;

  if (g.fget(in))
    goto end;

  out.open("gmpcc-out.txt");

  if (!out.is_open())
    goto end;

  g.bfs(sp - 1, [&g, &out](int v, int lvl) {
    g.vput(out, v);
    return true;
  });

  int spath;

  g.spp(sp - 1, ep - 1, spath);

  if (spath >= 0)
    cout << "Shortest path: " << spath << endl;
  else
    cout << "Path does not exists." << endl;

end:
  if (in.is_open())
    in.close();

  if (out.is_open())
    out.close();

  return 0;
}
