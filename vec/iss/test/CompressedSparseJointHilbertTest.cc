#include <gtest/gtest.h>

extern "C" {
#include <vec/iss_csj.h>

#include <math.h>
#include <stdio.h>
#include <time.h>
}

#define ALL(n) ((1 + (n - 1)) * (n - 1) / 2)

class Env : public testing::Environment {
 public:
  static struct mtx_csj_pps HilbertDefaultProp;
  static struct iss_csj_pkt HilbertDefaultPack;

  virtual void TearDown() {
    iss_csj_pkt_cls(&HilbertDefaultPack);
  }
};

struct iss_csj_pkt Env::HilbertDefaultPack = {
  .pkt = {
    .pps = 0,
    .x = 0,
    .f = fopen("mtx/hlb/f.vec", "r"),
  },
  .mtx = {
    .pps = 0,
    .dr = fopen("mtx/hlb/dr.csj.mtx", "r"),
    .lr = fopen("mtx/hlb/lr.csj.mtx", "r"),
    .ur = fopen("mtx/hlb/ur.csj.mtx", "r"),
    .ia = fopen("mtx/hlb/ia.csj.mtx", "r"),
    .ja = fopen("mtx/hlb/ja.csj.mtx", "r"),
  }
};

class CompressedSparseJointHilbertTest : public testing::Test {
 public:
  static struct iss_pps Properties;

  static double diff_ms(struct timespec* beg, struct timespec* end) {
    double sec = end->tv_sec - beg->tv_sec;
    double nan = abs(end->tv_nsec - beg->tv_nsec);

    return sec * 1000 + nan / 1000000;
  }

  void ContextDefault(FILE* rep, fun_iss_csj_slv slv);
  void ContextAll(FILE* rep, fun_iss_csj_slv slv);
};

struct iss_pps CompressedSparseJointHilbertTest::Properties = {1e-10, 100000};

void CompressedSparseJointHilbertTest::ContextDefault(FILE* rep, fun_iss_csj_slv slv) {
  struct timespec beg;
  struct timespec end;

  struct iss_res res = {0, 0};

  struct mtx_csj* mp = mtx_csj_new(3, 3);
  struct vec* xp = vec_new(mp->pps.n);
  struct vec* fp = vec_new(mp->pps.n);

  mtx_csj_fget(&Env::HilbertDefaultPack.mtx, mp);
  vec_get(Env::HilbertDefaultPack.pkt.f, fp);

  vec_zer(xp);

  clock_gettime(CLOCK_MONOTONIC, &beg);
  slv(mp, xp, fp, &Properties, &res, 0);
  clock_gettime(CLOCK_MONOTONIC, &end);

  EXPECT_GT(Properties.eps, fabs(res.res));
  EXPECT_GT(Properties.mk, res.k);

  fprintf(rep, "Iterations: %d\nResidual: %.7e\nTime: %lf ms\n\n", res.k,
          res.res, diff_ms(&beg, &end));

  vec_put(rep, xp);

  mtx_csj_cls(mp);
  vec_cls(xp);
  vec_cls(fp);
}

void CompressedSparseJointHilbertTest::ContextAll(FILE* rep, fun_iss_csj_slv slv) {
  struct timespec beg;
  struct timespec end;

  for (int n = 4; n <= 10; ++n) {
    struct iss_res res = {0, 0};

    struct mtx_csj* mp = mtx_csj_new(n, ALL(n));
    struct vec* xp = vec_new(n);
    struct vec* fp = vec_new(n);

    mtx_csj_hlb(mp);

    vec_seq(xp, 1);
    mtx_csj_vmlt(mp, xp, fp);
    vec_zer(xp);

    clock_gettime(CLOCK_MONOTONIC, &beg);
    slv(mp, xp, fp, &Properties, &res, 0);
    clock_gettime(CLOCK_MONOTONIC, &end);

    EXPECT_GT(Properties.eps, fabs(res.res));
    EXPECT_GT(Properties.mk, res.k);

    fprintf(rep, "N: %d\nIterations: %d\nResidual: %.7e\nTime: %lf ms\n\n", n,
            res.k, res.res, diff_ms(&beg, &end));

    mtx_csj_cls(mp);
    vec_cls(xp);
    vec_cls(fp);
  }
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  testing::AddGlobalTestEnvironment(new Env);

  return RUN_ALL_TESTS();
}
