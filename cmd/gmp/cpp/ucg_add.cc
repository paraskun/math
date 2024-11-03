#include <cerrno>
#include <cstring>
#include <fstream>
#include <iostream>

#include <gmp_ucg.hpp>

using namespace std;

int main(int argc, char** argv) {
  gmp_ucg m;
  gmp_ucg g;

  ifstream f;

  for (int i = 1; i < argc; ++i) {
    f.open(argv[i]);

    if (!f.is_open())
      goto end;

    if (g.fget_vtx(f))
      goto end;

    m.add(g);

    g.clear();
    f.close();
  }

  m.fput(cout);
  cout.flush();

end:
  if (errno)
    cout << strerror(errno) << endl;

  if (f.is_open())
    f.close();

  return 0;
}
