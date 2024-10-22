#ifndef GMP_ALS
#define GMP_ALS

#include <fstream>
#include <functional>
#include <vector>

struct gmp_als {
 private:
  std::vector<std::vector<int>> al;

  int fget(std::ifstream& f);
  int fput(std::ofstream& f);

  int bfs(int sp, std::function<bool(int, int)> f);
  int spp(int sp, int ep, int& r);
};

#endif  // GMP_ALS
