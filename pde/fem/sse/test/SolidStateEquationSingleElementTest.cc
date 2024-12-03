#include <gtest/gtest.h>

extern "C" {
#include <fem/sse/fem.h>
#include <vec/iss_csj.h>

#include <stdio.h>
};

class Env : public testing::Environment {
 public:
  static FILE* const obj;
  static struct iss_csj_pkt pkt;

  virtual void TearDown() {
    iss_csj_pkt_cls(&pkt);
    fclose(obj);
  }
};

FILE* const Env::obj = fopen("obj/sem.obj", "r");
struct iss_csj_pkt Env::pkt = {.pkt =
                                   {
                                       .pps = 0,
                                       .x = fopen("out/sem/vec/q.vec", "w+"),
                                       .f = fopen("out/sem/vec/b.vec", "w+"),
                                   },
                               .mtx = {
                                   .pps = 0,
                                   .dr = fopen("out/sem/mtx/dr.csj.mtx", "w+"),
                                   .lr = fopen("out/sem/mtx/lr.csj.mtx", "w+"),
                                   .ur = fopen("out/sem/mtx/ur.csj.mtx", "w+"),
                                   .ia = fopen("out/sem/mtx/ia.csj.mtx", "w+"),
                                   .ja = fopen("out/sem/mtx/ja.csj.mtx", "w+"),
                               }};

class SolidStateEquationSingleElementTest : public testing::Test {
 public:
  static double f(struct vtx* v) {
    double x = v->x;
    double y = v->y;
    double z = v->z;

    return 0.4 * (5 + 0.2 * x + y + 30 * z + 0.5 * x * y + x * z + 10 * y * z +
                  x * y * z);
  }

  static double g(struct vtx* v) {
    double x = v->x;
    double y = v->y;
    double z = v->z;

    return 2 * x + 0.04 * x * x + 0.4 * x * y + 12 * x * z + 0.1 * x * x * y +
           0.2 * x * x * z + 4 * x * y * z + 0.2 * x * x * y * z;
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

    return 4.5 - 0.05 * x + 25 * z + 0.5 * x * z;
  }

  static double tmp3(struct vtx* v) {
    double x = v->x;
    double y = v->y;

    return -10 - 0.3 * x - 4 * y;
  }

  double (*fun[7])(struct vtx*) = {&g,    &tta1, &tta2, &tta3,
                                   &tmp1, &tmp2, &tmp3};
};

TEST_F(SolidStateEquationSingleElementTest, GeneralTest) {
  struct fem* fem = fem_new(fun);

  fem_get(Env::obj, fem);
  fem_evo(fem);
  fem_asm(fem);

  struct vec* q = vec_new(fem->vs);

  fem_slv(fem, q);

  mtx_csj_put(&Env::pkt.mtx, fem->a);
  vec_put(Env::pkt.pkt.f, fem->b);
  vec_put(Env::pkt.pkt.x, q);

  fem_cls(fem);
  vec_cls(q);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  testing::AddGlobalTestEnvironment(new Env);

  return RUN_ALL_TESTS();
}
