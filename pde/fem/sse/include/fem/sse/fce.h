#ifndef FEM_FCE_H
#define FEM_FCE_H

#include <fem/sse/vtx.h>
#include <vec/mtx.h>
#include <vec/mtx_csj.h>

// Тип краевых условий
//  DIR - краевые условия первого типа (Дирихле)
//  NEU - краевые условия второго типа (Неймана)
//  ROB - краевые условия третьего типа (Робена)
enum type { DIR, NEU, ROB };

// Краевые условия
struct cnd {
  // Тип
  enum type type;

  // Параментры
  union pps {
    // Параметры для условий первого рода
    struct {
      double (*tmp)(struct vtx* v);
    } dir;

    // Параметры для условий второго рода
    struct {
      double (*tta)(struct vtx* v);
    } neu;

    // Параметры для условий третьего рода
    struct {
      double (*tmp)(struct vtx* v);
      double bet;
    } rob;
  } pps;
};

// Грань
struct fce {
  // Список из глобальных номеров узлов, составляющих данную грань
  int vtx[4];

  // Краевые условия
  struct cnd cnd;

  // Локальные матрица и / или вектор правой части
  union loc {
    // Локальный вектор правой части для условий второго рода
    struct {
      struct vec* b;
    } neu;

    // Локальные матрица и вектор правой части для условий третьего рода
    struct {
      struct vec* b;
      struct mtx* m;
    } rob;
  } loc;
};

// Подпрограмма выделения памяти под грань
struct fce* fce_new(enum type type);

// Подпрограмма чтения грани из строки
//  fun - список глобальных функций
struct fce* fce_get(const char* buf, double (**fun)(struct vtx*));

// Подпрограмма вычисления локальной матрицы и вектора правой части
//  v - глобальный список узлов
int fce_evo(struct fce* f, struct vtx** v);

// Подпрограмма занесения локальной матрицы 
// и вектора правой части в глобальную СЛАУ
//  a - матрица глобальной СЛАУ в строчно-столбцовом формате
//  b - вектор правой части глобальной СЛАУ
int fce_mov(struct fce* f, struct mtx_csj* a, struct vec* b);

int fce_cls(struct fce* f);

#endif  // FEM_FCE_H
