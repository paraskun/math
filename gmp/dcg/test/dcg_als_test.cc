#include <gtest/gtest.h>

extern "C" {
#include <dcg_als.h>
}

TEST(dcg_als, fwp) {
  FILE* rep = fopen("fwp.rep", "w+");

  struct dcg_als* g = dcg_als_new(4);

  dcg_als_add(g, 0, 1, 1);
  dcg_als_add(g, 0, 2, 6);
  dcg_als_add(g, 1, 2, 4);
  dcg_als_add(g, 1, 3, 1);
  dcg_als_add(g, 3, 2, 1);

  int* len[4];
  int* map[4];

  for (int i = 0; i < 4; ++i) {
    len[i] = (int*)malloc(sizeof(int) * 4);
    map[i] = (int*)malloc(sizeof(int) * 4);
  }

  dcg_als_fwp(g, len, map);
  dcg_als_map(rep, g->n, len, map);

  for (int i = 0; i < 4; ++i) {
    free(len[i]);
    free(map[i]);
  }

  dcg_als_cls(g);

  fclose(rep);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
