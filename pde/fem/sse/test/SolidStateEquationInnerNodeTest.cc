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

FILE* const Env::obj = fopen("obj/inn.obj", "r");

struct iss_csj_pkt Env::pkt = {
  .pkt = {
    .pps = 0,
    .x = fopen("out/inn/vec/q.vec", "w+"),
    .f = fopen("out/inn/vec/b.vec", "w+"),
  },
  .mtx = {
    .pps = 0,
    .dr = fopen("out/inn/mtx/dr.csj.mtx", "w+"),
    .lr = fopen("out/inn/mtx/lr.csj.mtx", "w+"),
    .ur = fopen("out/inn/mtx/ur.csj.mtx", "w+"),
    .ia = fopen("out/inn/mtx/ia.csj.mtx", "w+"),
    .ja = fopen("out/inn/mtx/ja.csj.mtx", "w+"),
  }
};

class SolidStateEquationInnerNodeTest : public testing::Test {
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

    return 2 * x + 0.04 * x * x + 0.4 * x * y + 
            12 * x * z + 0.1 * x * x * y +
            0.2 * x * x * z + 4 * x * y * z + 
            0.2 * x * x * y * z;
  }

  static double tmp0(struct vtx* v) { return 0 * v->x; }

  static double tta1(struct vtx* v) {
    double y = v->y;
    double z = v->z;

    return -1 - 2.5 * y - 5 * z - 5 * y * z;
  }

  static double tta2(struct vtx* v) {
    double y = v->y;
    double z = v->z;

    return 1 + 2.5 * y + 5 * z + 5 * y * z;
  }

  static double tmp1(struct vtx* v) {
    double x = v->x;
    double y = v->y;

    return 7 * x * y + 6.7 * x + 66 * y + 200;
  }

  static double tmp2(struct vtx* v) {
    double y = v->y;
    double z = v->z;

    return 13.5 * y * z + 2.75 * y + 33.5 * z + 5.7;
  }

  static double tmp3(struct vtx* v) {
    double x = v->x;
    double y = v->y;

    return 5 * x * y + 4.7 * x + 46 * y + 140;
  }

  double (*fun[7])(struct vtx*) = {
    &f,    
    &tmp0, 
    &tta1, &tta2,
    &tmp1, &tmp2, &tmp3
  };
};

TEST_F(SolidStateEquationInnerNodeTest, GeneralTest) {
  struct fem* fem = fem_new(fun);

  fem_get(Env::obj, fem);
  fem_evo(fem);
  fem_asm(fem);

  struct vec* q = vec_new(fem->vs);

  fem_slv(fem, q);

  mtx_csj_fput(&Env::pkt.mtx, fem->a);
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
