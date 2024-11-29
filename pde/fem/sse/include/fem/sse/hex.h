#ifndef FEM_HEX_H
#define FEM_HEX_H

#include <fem/sse/fce.h>
#include <fem/sse/vtx.h>
#include <vec/mtx.h>
#include <vec/mtx_csj.h>

// Прямоугольный параллелепипед
struct hex {
  // Список из глобальных номеров узлов, составляющих данный элемент
  int vtx[8];

  // Параметры
  struct {
    // Функция правой части
    double (*f)(struct vtx*);

    double lam;
    double gam;
  } pps;

  // Локальные матрица и вектор правой части
  struct {
    // Локальная матрица
    struct mtx* m;

    // Локальная правая часть
    struct vec* b;
  } loc;
};

// Подпрограмма выделения памяти под элемент
struct hex* hex_new();

// Подпрограмма чтения элемента из строки
//  fun - список глобальных функций
struct hex* hex_get(const char* buf, double (**fun)(struct vtx*));

// Подпрограмма вычисления локальной матрицы и вектора правой части
//  v - глобальный список узлов
int hex_evo(struct hex* h, struct vtx** v);

// Подпрограмма занесения локальной матрицы 
// и вектора правой части в глобальную СЛАУ
//  a - матрица глобальной СЛАУ в строчно-столбцовом формате
//  b - вектор правой части глобальной СЛАУ
int hex_mov(struct hex* h, struct mtx_csj* a, struct vec* b);

int hex_cls(struct hex* h);

#endif  // FEM_HEX_H
