#include <gtest/gtest.h>

extern "C" {
#include <dcg_als.h>
#include <time.h>
}

static double diff_ms(struct timespec* beg, struct timespec* end) {
  double sec = end->tv_sec - beg->tv_sec;
  double nan = abs(end->tv_nsec - beg->tv_nsec);

  return sec * 1000 + nan / 1000000;
}

TEST(dcg_sll, ins_end_exp) {
  struct dcg_sll* l = dcg_sll_new();

  dcg_sll_ins(l, &l->end, 1, 1);
  dcg_sll_ins(l, &l->end, 2, 2);
  dcg_sll_ins(l, &l->end, 3, 3);

  EXPECT_EQ(3, l->s);
  EXPECT_EQ(3, l->end->vtx);

  struct dcg_edg* e = l->beg;

  for (int i = 0; i < 3; ++i) {
    EXPECT_EQ(i + 1, e->vtx);
    EXPECT_EQ(i + 1, e->wgt);

    e = e->next;
  }

  dcg_sll_cls(l);
}

TEST(dcg_sll, ins_end_imp) {
  struct dcg_sll* l = dcg_sll_new();
  struct dcg_edg* e = NULL;

  dcg_sll_ins(l, &e, 1, 1);
  dcg_sll_ins(l, &e, 2, 2);
  dcg_sll_ins(l, &e, 3, 3);

  EXPECT_EQ(3, l->s);
  EXPECT_EQ(3, l->end->vtx);

  e = l->beg;

  for (int i = 0; i < 3; ++i) {
    EXPECT_EQ(i + 1, e->vtx);
    EXPECT_EQ(i + 1, e->wgt);

    e = e->next;
  }

  dcg_sll_cls(l);
}

TEST(dcg_sll, ins_btw) {
  struct dcg_sll* l = dcg_sll_new();
  struct dcg_edg* e = NULL;

  dcg_sll_ins(l, &e, 1, 1);
  dcg_sll_ins(l, &l->end, 3, 3);
  dcg_sll_ins(l, &e, 2, 2);

  EXPECT_EQ(3, l->s);
  EXPECT_EQ(3, l->end->vtx);

  e = l->beg;

  for (int i = 0; i < 3; ++i) {
    EXPECT_EQ(i + 1, e->vtx);
    EXPECT_EQ(i + 1, e->wgt);

    e = e->next;
  }

  dcg_sll_cls(l);
}

TEST(dcg_sll, ins_beg) {
  struct dcg_sll* l = dcg_sll_new();
  struct dcg_edg* e = NULL;

  dcg_sll_ins(l, &e, 3, 3);
  e = NULL;
  dcg_sll_ins(l, &e, 2, 2);
  e = NULL;
  dcg_sll_ins(l, &e, 1, 1);

  EXPECT_EQ(3, l->s);
  EXPECT_EQ(3, l->end->vtx);

  e = l->beg;

  for (int i = 0; i < 3; ++i) {
    EXPECT_EQ(i + 1, e->vtx);
    EXPECT_EQ(i + 1, e->wgt);

    e = e->next;
  }

  dcg_sll_cls(l);
}

TEST(dcg_sll, add) {
  struct dcg_sll* l = dcg_sll_new();

  dcg_sll_add(l, 3, 3);
  dcg_sll_add(l, 1, 1);
  dcg_sll_add(l, 2, 2);

  EXPECT_EQ(3, l->s);
  EXPECT_EQ(3, l->end->vtx);

  struct dcg_edg* e = l->beg;

  for (int i = 0; i < 3; ++i) {
    EXPECT_EQ(i + 1, e->vtx);
    EXPECT_EQ(i + 1, e->wgt);

    e = e->next;
  }

  dcg_sll_cls(l);
}

TEST(dcg_als, fwp_conn) {
  FILE* rep = fopen("rep/fwp_conn.rep", "w+");
  FILE* fin = fopen("dcg/conn.dcg", "r");

  int n;

  fscanf(fin, "%d", &n);

  struct dcg_als* g = dcg_als_new(n);

  dcg_als_get(fin, g, 1);

  struct timespec beg;
  struct timespec end;

  clock_gettime(CLOCK_MONOTONIC, &beg);
  dcg_als_fwp(g, 1);
  clock_gettime(CLOCK_MONOTONIC, &end);

  fprintf(rep, "Time: %.7e ms.\n\n", diff_ms(&beg, &end));

  dcg_als_map(rep, g);
  dcg_als_cls(g);

  fclose(rep);
  fclose(fin);
}

TEST(dcg_als, fwp_comp) {
  FILE* rep = fopen("rep/fwp_comp.rep", "w+");
  FILE* fin = fopen("dcg/comp.dcg", "r");

  int n;

  fscanf(fin, "%d", &n);

  struct dcg_als* g = dcg_als_new(n);

  dcg_als_get(fin, g, 1);

  struct timespec beg;
  struct timespec end;

  clock_gettime(CLOCK_MONOTONIC, &beg);
  dcg_als_fwp(g, 1);
  clock_gettime(CLOCK_MONOTONIC, &end);

  fprintf(rep, "Time: %.7e ms.\n\n", diff_ms(&beg, &end));

  dcg_als_map(rep, g);
  dcg_als_cls(g);

  fclose(rep);
  fclose(fin);
}

TEST(dcg_als, fwp_full) {
  FILE* rep = fopen("rep/fwp_full.rep", "w+");
  FILE* fin = fopen("dcg/full.dcg", "r");

  int n;

  fscanf(fin, "%d", &n);

  struct dcg_als* g = dcg_als_new(n);

  dcg_als_get(fin, g, 1);

  struct timespec beg;
  struct timespec end;

  clock_gettime(CLOCK_MONOTONIC, &beg);
  dcg_als_fwp(g, 1);
  clock_gettime(CLOCK_MONOTONIC, &end);

  fprintf(rep, "Time: %.7e ms.\n\n", diff_ms(&beg, &end));

  dcg_als_map(rep, g);
  dcg_als_cls(g);

  fclose(rep);
  fclose(fin);
}

TEST(dcg_als, fwp_fbig) {
  FILE* rep = fopen("rep/fwp_fbig.rep", "w+");
  FILE* fin = fopen("dcg/fbig.dcg", "r");

  int n;

  fscanf(fin, "%d", &n);

  struct dcg_als* g = dcg_als_new(n);

  dcg_als_get(fin, g, 1);

  struct timespec beg;
  struct timespec end;

  clock_gettime(CLOCK_MONOTONIC, &beg);
  dcg_als_fwp(g, 0);
  clock_gettime(CLOCK_MONOTONIC, &end);

  double nrm;

  dcg_als_nrm(g, &nrm);

  fprintf(rep, "Time: %.7e ms.\n", diff_ms(&beg, &end));
  fprintf(rep, "Norm: %lf\n", nrm);

  dcg_als_cls(g);

  fclose(rep);
  fclose(fin);
}

TEST(dcg_als, fwp_sbig) {
  FILE* rep = fopen("rep/fwp_sbig.rep", "w+");
  FILE* fin = fopen("dcg/sbig.dcg", "r");

  int n;

  fscanf(fin, "%d", &n);

  struct dcg_als* g = dcg_als_new(n);

  dcg_als_get(fin, g, 1);

  struct timespec beg;
  struct timespec end;

  clock_gettime(CLOCK_MONOTONIC, &beg);
  dcg_als_fwp(g, 0);
  clock_gettime(CLOCK_MONOTONIC, &end);

  double nrm;

  dcg_als_nrm(g, &nrm);

  fprintf(rep, "Time: %.7e ms.\n", diff_ms(&beg, &end));
  fprintf(rep, "Norm: %lf\n", nrm);

  dcg_als_cls(g);

  fclose(rep);
  fclose(fin);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
