#include "pch.h"
#include <iostream>
#include <conio.h>
#include <iomanip>
#include <math.h>
#include <stdio.h>

using namespace std;

int n = 3;
double xk = 1; // Конечный Х

 // инициализация начальных данных
void standart(double *y, double &x, double &h)
{
	setlocale(0, "");
	y[0] = 1;	// y(x) = 1
	y[1] = 1;	// y'(x) = 1
	x = 0;		// x = 0
	h = 0.1;	// Шаг прохода по Х
	
}

 // Сумма матриц
double* sum(double *a, double *b)
{
	for (int i = 0; i < 2; i++) {
		a[i] = a[i] + b[i];
	}
	return a;
}

 // Умножение матрици на число
void multiply(double h, double *f, double *result)
{
	result[0] = h * f[0];
	result[1] = h * f[1];
}

 // присвоение одной матрицы другой
void equal(double *a, double *b)
{
	a[0] = b[0];
	a[1] = b[1];
}

 // ГЛАВНАЯ ФУНКЦИЯ для изменения
double* F(double x, double *y, double *f)
{
	f[0] = y[1];
	f[1] = (exp(x) + y[0]) / 2;
	return f;
}

 // Для р-к 2 по времени y*
void y_star(double *y, double x, double *f, double h)
{
	double multer[2];

	F(x, y, f);
	multiply(h / 2, f, multer);
	sum(y, multer);
}

 // Р-К 2 по времени
double* RungeKutta_Time(double * y, double & x, double * f, double h)
{
	double multer[2], y_prev[2];

	equal(y_prev, y);
	y_star(y_prev, x, f, h);
	F(x + h / 2, y_prev, f);
	multiply(h, f, multer);
	sum(y, multer);
	x += h;

	return y;
}

 // Р-К 4 порядка
double* RungeKutta_4th(double * y, double & x, double * f, double h)
{
	double multer[2], y_prev[2];
	double k[2], k_prev[2];
	double summer[2] = { 0, 0 };

	 // К1
	F(x, y, f);
	equal(k, f);
	sum(summer, k);

	 // К2 и К3
	for (int j = 0; j < 2; j++) {
		multiply(h / 2, k, multer);
		equal(y_prev, y);
		sum(y_prev, multer);
		F(x + h / 2, y_prev, f);
		equal(k, f);
		sum(summer, k);
		sum(summer, k);
	}

	 // К4
	multiply(h, k, multer);
	equal(y_prev, y);
	sum(y_prev, multer);
	F(x + h, y_prev, f);
	equal(k, f);
	sum(summer, k);

	 // У
	multiply(h / 6, summer, multer);
	sum(y, multer);

	x += h;

	return y;
}

 // Вывод таблицы
void Output_result(double * m, double x, int i, double h)
{
	if (i==1)
	cout << x <<"  "<< m[0] << "  "<< m[1] << "  " << h << endl;
	if (i == 2)
		cout << x << "  " << m[0] << "  " << m[1] << "  " << h << endl;
	
	if (i == 0)
		cout << x << "  " << m[0] << "  " << m[1] << "  " << h << endl;
}

 // Условие выхода из Двойного пересчета
double okr(double **y, double **yy, int N, double eps) {
	double t;
	for (int i = 0; i <= N; i += 2) {
		if (fabs((y[i / 2][1] - yy[i][1])) < 15 * eps) {
			t = 0;
		}
		else t = fabs((y[i / 2][1] - yy[i][1]));
	}

	return t;
}

int main()
{	
	double y[2];
	double x;
	double f[2];

	double y1[2];
	double x1;
	double f1[2];

	double e = 0.00000001; // Точность
	double h; // Полный шаг
	double h_emty;
	double sub_h; // Половина шага

	standart(y, x, h);
	
	cout << fixed << setprecision(10);
	 // Инициализация значений
	standart(y, x, h);
	standart(y1, x1, sub_h);
	sub_h /= 2;
	double step = 0;

	char key = '9';
	while (true) {
		system("cls");
		printf("1 - P-K по времени\n");
		printf("2 - P-K 4 порядка\n");
		key = _getch();
		switch (key) {
		case '1':
			 // Инициализация
			standart(y, x, h);
			standart(y1, x1, sub_h);
			sub_h /= 2;  // Берем шаг в 2 раза меньше
			step=0;
			do {
				 // Инициализация
				standart(y1, x1, h_emty);
				standart(y, x, h_emty);
				cout << endl;
				cout << "H = " << h << endl;
				cout << "H/2 = " << sub_h << endl;
				for (step = x; step < xk; step += h) { // от начального Х до Конечного с полным шагом
					RungeKutta_Time(y, x, f, h); // Сам Р-К по времени
					if (round(x * 100000) / (100000) > xk) break;
					//Output_result(y1, x1, 2, h);
				}
		
				cout << endl;

				for (step = x1; step < xk; step +=sub_h) {  // с Шагом в 2 раза меньше
					RungeKutta_Time(y1, x1, f1, sub_h);	 // Еще раз Р-К по времени		
					if (round(x1 * 100000) / (100000) > xk) break;
					Output_result(y1, x1, 2, sub_h); // Вывод только половинчатого шага
				}

				 // Уменьшаем
				h = sub_h;
				sub_h = h / 2;

			} while (fabs(y[0] - y1[0]) > 15 * e); // Пока разница между У не будет меньше 15*е мы считаем
			system("pause");
			break;
		case '2':
			 // Инициализация
			standart(y, x, h);
			standart(y1, x1, sub_h);
			sub_h /= 2;
			step=0;
			do {	
				 // Инициализация
				standart(y1, x1, h_emty);
				standart(y, x, h_emty);
				cout << endl;
				cout << "H = " << h << endl;
				cout << "H/2 = " << sub_h << endl;
				for (step = x; step < xk; step += h) { // Полный шаг
					RungeKutta_4th(y, x, f, h); // 4 порядка
					if (round(x * 100000) / (100000) > xk) break;
					//Output_result(y1, x1, 2, h);
				}
		
				cout << endl;

				for (step = x1; step < xk; step +=sub_h) { // Половина шага
					RungeKutta_4th(y1, x1, f1, sub_h); // 4 порядка
					if (round(x1 * 100000) / (100000) > xk) break;
					Output_result(y1, x1, 2, sub_h);
				}
				 // Уменьшаем шаги
				h = sub_h;
				sub_h = h / 2;
			} while (fabs(y[0] - y1[0]) > 15 * e); // Пока разница между У не будет меньше 15*е мы считаем
			system("pause");
			break;
		case 'q': // Выход
			return 0;
		default:
			break;
		}
	}
	return 0;
}


