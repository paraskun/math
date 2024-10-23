#ifndef GMP_ALS
#define GMP_ALS

#include <fstream>
#include <functional>
#include <list>
#include <vector>

struct gmp_vtx {
  int v;
  int p;
};

struct gmp_ucg {
 private:
  std::vector<std::vector<int>> al;

 public:
  int fget(std::ifstream& f);
  int fput(std::ofstream& f);
  int vput(std::ofstream& f, int v);

  int bfs(int sp, std::function<bool(gmp_vtx)> f);
  int spp(int sp, int ep, std::list<int>& path);
};

#endif  // GMP_ALS
