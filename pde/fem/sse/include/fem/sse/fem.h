#ifndef FEM_H
#define FEM_H

#include <fem/sse/hex.h>

#include <vec/mtx.h>
#include <vec/mtx_csj.h>

// Структура алгоритма
struct fem {
  int vs; // Количество вершин
  int hs; // Количество элементов
  int fs; // Количество граней

  // Список вершин
  struct vtx** vtx;

  // Список элементов
  struct hex** hex;

  // Список граней
  struct fce** fce;

  // Параметры
  struct {
    // Список функций
    double (**fun)(struct vtx*);
  } pps;

  // Матрица СЛАУ
  struct mtx_csj* a;

  // Вектор правой части СЛАУ
  struct vec* b;
};

// Подпрограмма выделения памяти
struct fem* fem_new(double (**fun)(struct vtx*));

// Подпрограмма инициализации из файла
int fem_get(FILE* obj, struct fem* fem);

// Подпрограмма вычисления локальных матриц и векторов
// конечных элементов и граней с заданными краевыми
// условиями
int fem_evo(struct fem* fem);

// Подпрограмма сборки СЛАУ
int fem_asm(struct fem* fem);

// Подпрограмма решения СЛАУ
//  q - результирующий вектор
int fem_slv(struct fem* fem, struct vec* q);

int fem_cls(struct fem* fem);

#endif  // FEM_H
