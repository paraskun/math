#include <gtest/gtest.h>

extern "C" {
#include <vec/iss_csj.h>

#include <math.h>
#include <stdio.h>
#include <time.h>
}

class Env : public testing::Environment {
 public:
  static struct mtx_csj_pps DominantProperties;
  static struct iss_csj_pkt DominantPack;

  virtual void SetUp() {
    FILE* f = fopen("mtx/ddm/f.vec", "w+");

    struct mtx_csj* mp = mtx_csj_new(Env::DominantProperties);
    struct vec* xp = vec_new(mp->pps.n);
    struct vec* fp = vec_new(mp->pps.n);

    mtx_csj_get(&Env::DominantPack.mtx, mp);
    vec_seq(xp, 1);
    mtx_csj_vmlt(mp, xp, fp);
    vec_put(f, fp);

    mtx_csj_cls(mp);
    vec_cls(xp);
    vec_cls(fp);
  }

  virtual void TearDown() { iss_csj_pkt_cls(&DominantPack); }
};

struct mtx_csj_pps Env::DominantProperties = {10, 17};
struct iss_csj_pkt Env::DominantPack = {
    .pkt =
        {
            .pps = 0,
            .x = 0,
            .f = fopen("mtx/ddm/f.vec", "r"),
        },
    .mtx = {
        .pps = 0,
        .dr = fopen("mtx/ddm/dr.csj.mtx", "r"),
        .lr = fopen("mtx/ddm/lr.csj.mtx", "r"),
        .ur = fopen("mtx/ddm/ur.csj.mtx", "r"),
        .ia = fopen("mtx/ddm/ia.csj.mtx", "r"),
        .ja = fopen("mtx/ddm/ja.csj.mtx", "r"),
    }};

class CompressedSparseJointDominantTest : public testing::Test {
 public:
  static struct iss_pps Properties;

  static double diff_ms(struct timespec* beg, struct timespec* end) {
    double sec = end->tv_sec - beg->tv_sec;
    double nan = abs(end->tv_nsec - beg->tv_nsec);

    return sec * 1000 + nan / 1000000;
  }

  void Context(FILE* rep, fun_iss_csj_slv slv);
};

struct iss_pps CompressedSparseJointDominantTest::Properties = {1e-10, 100000};

void CompressedSparseJointDominantTest::Context(FILE* rep,
                                                fun_iss_csj_slv slv) {
  struct timespec beg;
  struct timespec end;

  struct iss_res res = {0, 0};

  struct mtx_csj* mp = mtx_csj_new(Env::DominantProperties);
  struct vec* xp = vec_new(mp->pps.n);
  struct vec* fp = vec_new(mp->pps.n);

  mtx_csj_get(&Env::DominantPack.mtx, mp);
  vec_get(Env::DominantPack.pkt.f, fp);

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

TEST_F(CompressedSparseJointDominantTest, LocalOptimalTest) {
  FILE* rep = fopen("out/los_ddm_def.rep", "w+");

  Context(rep, &iss_csj_los_slv);

  fclose(rep);
}

TEST_F(CompressedSparseJointDominantTest, LocalOptimalIncompleteLUTest) {
  FILE* rep = fopen("out/los_ilu_ddm_def.rep", "w+");

  Context(rep, &iss_csj_ilu_los_slv);

  fclose(rep);
}

TEST_F(CompressedSparseJointDominantTest, LocalOptimalDiagonalTest) {
  FILE* rep = fopen("out/los_dgl_ddm_def.rep", "w+");

  Context(rep, &iss_csj_dgl_los_slv);

  fclose(rep);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  testing::AddGlobalTestEnvironment(new Env);

  return RUN_ALL_TESTS();
}
