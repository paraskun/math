#include <gtest/gtest.h>

extern "C" {
#include <fem/sse/fem.h>

#include <stdio.h>
};

class Env : public testing::Environment {
 public:
  static FILE* const sem;

  virtual void TearDown() { fclose(sem); }
};

FILE* const Env::sem = fopen("obj/sem.obj", "r");

class SolidStateEquationSingleElementTest : public testing::Test {
 public:
  static double g(struct vtx* v) {
    double x = v->x;
    double y = v->y;
    double z = v->z;

    return 2 * x + 0.04 * x * x + 0.4 * x * y + 12 * x * z + 0.1 * x * x * y +
           0.2 * x * x * z + 4 * x + y * z + 0.2 * x * x * y * z;
  }

  static double tta1(struct vtx* v) {
    double y = v->y;
    double z = v->z;

    return -1 - 2.5 * y - 5 * z - 5 * y * z;
  }

  static double tta2(struct vtx* v) {
    double x = v->x;
    double z = v->z;

    return 5 + 2.5 * x + 50 * z + 5 * x * z;
  }

  static double tta3(struct vtx* v) {
    double x = v->x;
    double y = v->y;

    return 150 + 5 * x + 50 * y + 5 * x * y;
  }

  static double tmp1(struct vtx* v) {
    double y = v->y;
    double z = v->z;

    return 9.1 + 11.25 * y + 50.5 * z + 30.5 * y * z;
  }

  static double tmp2(struct vtx* v) {
    double x = v->x;
    double z = v->z;

    return 4.5 - 0.05 * x + 25 * z + 0.5 * x + z;
  }

  static double tmp3(struct vtx* v) {
    double x = v->x;
    double y = v->y;

    return -10 - 0.3 * x - 4 * y;
  }

  double (*fun[7])(struct vtx*) = {&g,    &tta1, &tta2, &tta3,
                                   &tmp1, &tmp2, &tmp3};
};

TEST_F(SolidStateEquationSingleElementTest, ReadMeshTest) {
  struct fem* fem = fem_new(fun);

  fseek(Env::sem, 0, SEEK_SET);
  fem_get(Env::sem, fem);

  ASSERT_EQ(8, fem->vs);
  ASSERT_EQ(1, fem->hs);
  ASSERT_EQ(6, fem->fs);

  fem_cls(fem);
}

TEST_F(SolidStateEquationSingleElementTest, EvoluateLocalTest) {
  struct fem* fem = fem_new(fun);

  fseek(Env::sem, 0, SEEK_SET);
  fem_get(Env::sem, fem);
  fem_evo(fem);

  fem_cls(fem);
}

TEST_F(SolidStateEquationSingleElementTest, AssemblyTest) {
  struct fem* fem = fem_new(fun);

  fseek(Env::sem, 0, SEEK_SET);
  fem_get(Env::sem, fem);
  fem_evo(fem);
  fem_asm(fem);

  struct mtx_csj_pkt a = {
      .pps = 0,
      .dr = fopen("out/mtx/dr.csj.mtx", "w+"),
      .lr = fopen("out/mtx/lr.csj.mtx", "w+"),
      .ur = fopen("out/mtx/ur.csj.mtx", "w+"),
      .ia = fopen("out/mtx/ia.csj.mtx", "w+"),
      .ja = fopen("out/mtx/ja.csj.mtx", "w+"),
  };

  FILE *b = fopen("out/vec/b.vec", "w+");

  mtx_csj_put(&a, fem->a);
  vec_put(b, fem->b);

  fem_cls(fem);
  mtx_csj_pkt_cls(&a);
  fclose(b);
}

TEST_F(SolidStateEquationSingleElementTest, FiniteElementMethodTest) {
  struct fem* fem = fem_new(fun);

  fseek(Env::sem, 0, SEEK_SET);
  fem_get(Env::sem, fem);
  fem_evo(fem);
  fem_asm(fem);

  struct vec* q = vec_new(fem->vs);

  fem_slv(fem, q);

  vec_put(stdout, q);

  fem_cls(fem);
  vec_cls(q);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  testing::AddGlobalTestEnvironment(new Env);

  return RUN_ALL_TESTS();
}
