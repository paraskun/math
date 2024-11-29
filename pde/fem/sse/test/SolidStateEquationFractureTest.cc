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

FILE* const Env::obj = fopen("obj/fct.obj", "r");

struct iss_csj_pkt Env::pkt = {
  .pkt = {
    .pps = 0,
    .x = fopen("out/fct/vec/q.vec", "w+"),
    .f = fopen("out/fct/vec/b.vec", "w+"),
  },
  .mtx = {
    .pps = 0,
    .dr = fopen("out/fct/mtx/dr.csj.mtx", "w+"),
    .lr = fopen("out/fct/mtx/lr.csj.mtx", "w+"),
    .ur = fopen("out/fct/mtx/ur.csj.mtx", "w+"),
    .ia = fopen("out/fct/mtx/ia.csj.mtx", "w+"),
    .ja = fopen("out/fct/mtx/ja.csj.mtx", "w+"),
  }
};

class SolidStateEquationFractureTest : public testing::Test {
 public:
  static double f1(struct vtx* v) {
    double x = v->x;
    double y = v->y;
    double z = v->z;

    return 8 * x * (y * z + z + y + 1);
  }

  static double f2(struct vtx* v) {
    return 0 * v->x;
  }

  static double f3(struct vtx* v) {
    double x = v->x;
    double y = v->y;
    double z = v->z;

    return 3 * x * z * (y + 1);
  }

  static double tmp0(struct vtx* v) { return 0 * v->x; }

  static double tta1(struct vtx* v) {
    double x = v->x;
    double z = v->z;

    return -2 * x * (z + 1);
  }

  static double tta2(struct vtx* v) {
    double x = v->x;
    double z = v->z;

    return -2 * x * z;
  }

  static double tta3(struct vtx* v) {
    double x = v->x;
    double z = v->z;

    return -2 * (1 + x + z + x * z);
  }

  static double tta4(struct vtx* v) {
    double y = v->y;
    double z = v->z;

    return 2 * (1 + y + z + y * z);
  }

  static double tta5(struct vtx* v) {
    double x = v->x;
    double y = v->y;

    return 2 * (1 + x + y + x * y);
  }

  static double tta6(struct vtx* v) {
    double y = v->y;
    double z = v->z;

    return 2 * z * (1 + y);
  }

  static double tta7(struct vtx* v) {
    double x = v->x;
    double y = v->y;

    return 2 * x * (1 + y);
  }

  static double tta8(struct vtx* v) {
    double x = v->x;
    double y = v->y;

    return -2 * x * (1 + y);
  }

  static double tta9(struct vtx* v) {
    double x = v->x;
    double y = v->y;

    return -2 * (1 + x + y + x * y);
  }

  static double tmp1(struct vtx* v) {
    double x = v->x;
    double z = v->z;

    return 8 * x * (z + 1);
  }

  static double tmp2(struct vtx* v) {
    double x = v->x;
    double z = v->z;

    return 11.5 * x * z;
  }

  static double tmp3(struct vtx* v) {
    double x = v->x;
    double z = v->z;

    return 4.5 * (1 + x + z + x * z);
  }

  double (*fun[16])(struct vtx*) = {
    &f1, &f2, &f3,
    &tmp0,
    &tta1, &tta2, &tta3, &tta4, &tta5, &tta6, &tta7, &tta8, &tta9,
    &tmp1, &tmp2, &tmp3
  };
};

TEST_F(SolidStateEquationFractureTest, GeneralTest) {
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
