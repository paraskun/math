#ifndef GMP_UCG
#define GMP_UCG

#include <functional>
#include <istream>
#include <list>
#include <vector>

struct gmp_ucg {
 private:
  std::vector<std::list<int>> al;

 public:
  int add(int a, int b);
  int add(gmp_ucg& g);

  int fget_edg(std::istream& f);
  int fget_vtx(std::istream& f);

  int fput(std::ostream& f);
  int vput(std::ostream& f, int v);

  int bfs(int sp, std::function<bool(int, int)> f);
  int spp(int sp, int ep, std::list<int>& path);

  void clear();
};

#endif  // GMP_UCG
