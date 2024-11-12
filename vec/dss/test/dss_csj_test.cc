#include <gtest/gtest.h>

extern "C" {
#include <dss_csj.h>
}

TEST(dss_csj, ilu_lslv) {
  struct mtx_csj_pps pps = {
      .n = 3,
      .le = 3,
      .ue = 0,
  };

  //  1  -  -
  // -1  2  -
  // 10 -2  3
  struct mtx_csj* ilu = mtx_csj_new(pps);

  ilu->dr[0] = 1;
  ilu->dr[1] = 2;
  ilu->dr[2] = 3;

  ilu->lr[0] = -1;
  ilu->lr[1] = 10;
  ilu->lr[2] = -2;

  ilu->il[0] = 0;
  ilu->il[1] = 0;
  ilu->il[2] = 1;
  ilu->il[3] = 3;

  ilu->jl[0] = 0;
  ilu->jl[1] = 0;
  ilu->jl[2] = 1;

  struct vec* xp = vec_new(3);
  struct vec* fp = vec_new(3);

  fp->vp[0] = 1;
  fp->vp[1] = 2;
  fp->vp[2] = 3;

  iss_csj_ilu_lslv(ilu, xp, fp);

  EXPECT_FLOAT_EQ(1.0, xp->vp[0]);
  EXPECT_FLOAT_EQ(1.5, xp->vp[1]);
  EXPECT_FLOAT_EQ(-4.0 / 3.0, xp->vp[2]);

  mtx_csj_free(ilu);
  vec_free(xp);
  vec_free(fp);
}

TEST(dss_csj, ilu_uslv) {
  struct mtx_csj_pps pps = {
      .n = 3,
      .le = 0,
      .ue = 3,
  };

  //  1 -1 10
  //  -  2 -2
  //  -  -  3
  struct mtx_csj* ilu = mtx_csj_new(pps);

  ilu->dr[0] = 1;
  ilu->dr[1] = 2;
  ilu->dr[2] = 3;

  ilu->ur[0] = -1;
  ilu->ur[1] = 10;
  ilu->ur[2] = -2;

  ilu->ju[0] = 0;
  ilu->ju[1] = 0;
  ilu->ju[2] = 1;
  ilu->ju[3] = 3;

  ilu->iu[0] = 0;
  ilu->iu[1] = 0;
  ilu->iu[2] = 1;

  struct vec* xp = vec_new(3);
  struct vec* fp = vec_new(3);

  fp->vp[0] = 1;
  fp->vp[1] = 2;
  fp->vp[2] = 3;

  iss_csj_ilu_uslv(ilu, xp, fp);

  EXPECT_FLOAT_EQ(-7.0, xp->vp[0]);
  EXPECT_FLOAT_EQ(2.0, xp->vp[1]);
  EXPECT_FLOAT_EQ(1.0, xp->vp[2]);

  mtx_csj_free(ilu);
  vec_free(xp);
  vec_free(fp);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
