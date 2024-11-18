#include <gtest/gtest.h>

extern "C" {
#include <mtx_csj.h>
}

TEST(mtx_csj, vmlt) {
  struct mtx_csj_pps pps = {
      .n = 3,
      .ne = 2,
  };

  //  1  4  5
  // -1  2  0
  //  1  0  3
  struct mtx_csj* mp = mtx_csj_new(pps);

  mp->dr[0] = 1;
  mp->dr[1] = 2;
  mp->dr[2] = 3;

  mp->lr[0] = -1;
  mp->lr[1] = 1;

  mp->ur[0] = 4;
  mp->ur[1] = 5;

  mp->ia[0] = 0;
  mp->ia[1] = 0;
  mp->ia[2] = 1;
  mp->ia[3] = 2;

  mp->ja[0] = 0;
  mp->ja[1] = 0;

  // (1, -2, 3)
  struct vec* xp = vec_new(3);
  struct vec* fp = vec_new(3);

  xp->vp[0] = 1;
  xp->vp[1] = -2;
  xp->vp[2] = 3;

  mtx_csj_vmlt(mp, xp, fp);

  EXPECT_FLOAT_EQ(8.0, fp->vp[0]);
  EXPECT_FLOAT_EQ(-5.0, fp->vp[1]);
  EXPECT_FLOAT_EQ(10.0, fp->vp[2]);

  mtx_csj_cls(mp);
  vec_cls(xp);
  vec_cls(fp);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
