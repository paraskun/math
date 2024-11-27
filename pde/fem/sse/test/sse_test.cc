#include <gtest/gtest.h>

extern "C" {
#include <stdio.h>

#include <fem/sse/fem.h>
};

class Env : public testing::Environment {
 public:
  static FILE* inn;
  static FILE* smp;

  virtual void SetUp() {
    inn = fopen("obj/inn.obj", "r");
    smp = fopen("obj/smp.obj", "r");
  }

  virtual void TearDown() {
    fclose(inn);
    fclose(smp);
  }
};

class SolidStateEquationTest : testing::Test {
 public:
  static double smp_tta_1(struct vtx* v) {
    return -1 - 2.5 * v->y - 5 * v->z - 5 * v->y * v->z;
  }

  static double smp_tta_2(struct vtx* v) {
    return 5 + 2.5 * v->x + 50 * v->z + 5 * v->x * v->z;
  }

  static double smp_tta_3(struct vtx* v) {
    return 150 + 5 * v->x + 50 * v->y + 5 * v->x * v->y;
  }

  static double smp_tmp_1(struct vtx* v) {
    return 9.1 + 11.25 * v->y + 50.5 * v->z + 30.5 * v->y * v->z;
  }

  static double smp_tmp_2(struct vtx* v) {
    return 4.5 - 0.05 * v->x + 25 * v->z + 0.5 * v->x + v->z;
  }

  static double smp_tmp_3(struct vtx* v) {
    return -10 - 0.3 * v->x - 4 * v->y;
  }
};

TEST_F(SolidStateEquationTest, SingleElementMeshTest) {
  double (*fun[6])(struct vtx*) = {
      &smp_tta_1, &smp_tta_2, &smp_tta_3, &smp_tmp_1, &smp_tmp_2, &smp_tmp_3,
  };

  struct fem* fem = fem_new(fun);

  fem_get(Env::smp, fem);
  fem_evo(fem);
  fem_asm(fem);

  struct vec* q = vec_new(fem->vs);

  fem_slv(fem, q);
  fem_cls(fem);
}

TEST_F(SolidStateEquationTest, InnerElementMeshTest) {
  double (*fun[3])(struct vtx*) = {};

  struct fem* fem = fem_new(fun);

  fem_get(Env::smp, fem);
  fem_evo(fem);
  fem_asm(fem);

  struct vec* q = vec_new(fem->vs);

  fem_slv(fem, q);
  fem_cls(fem);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  testing::AddGlobalTestEnvironment(new Env);

  return RUN_ALL_TESTS();
}
