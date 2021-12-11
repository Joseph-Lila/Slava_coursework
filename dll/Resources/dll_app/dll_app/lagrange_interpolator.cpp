#include "pch.h"
#include "lagrange_interpolator.h"
#include "my_library.h"

/// <summary>
/// Вычисление значений интерполирующей функции
/// </summary>
/// <param name="xValues">Массив значений x исходной функции</param>
/// <param name="yValues">Массив значений н исходной функции</param>
/// <param name="initialCount">Размер массивов значений исходной функции</param>
/// <param name="start">Начальное значение x интерполирующей функции</param>
/// <param name="end">Конечное значение x интерполирующей функции</param>
/// <param name="dotsCount">Требуемое количество значений интерполирующей функции</param>
/// <returns>Статус результата.</returns>
int LagrangeInterpolate(double* xValues, double* yValues, int initialCount, double start, double end, int dotsCount, double* xBuffer, double* yBuffer) {
	//Вполне себе работает, но не доходит до последнего элемента.
	/*int j = 0;
	for (double x = start; x < end; x += (end - start) / (dotsCount - 1)) {
		xBuffer[j] = x;
		yBuffer[j] = InterpolateLagrangePolynomial(xValues, yValues, initialCount, x);
		j++;
	}*/

	// Доходит до последнего элемента.
	double x = start;
	double deltaX = (end - start) / (dotsCount - 1);
	for (int j = 0; j < dotsCount; j++) {
		xBuffer[j] = x;
		yBuffer[j] = InterpolateLagrangePolynomial(xValues, yValues, initialCount, x);
		x += deltaX;
	}
	return 1;
}

/// <summary>
/// Вычисление значения интерполирующей функции для данного значения аргумента _x и исходной функции.
/// </summary>
/// <param name="xValues">Массив значений x исходной функции</param>
/// <param name="yValues">Массив значений y исходной функции</param>
/// <param name="n">Степень точности</param>
/// <param name="_x">Значение аргумента x</param>
/// <returns>Значение интерполирующей функции для переданного _x</returns>
double InterpolateLagrangePolynomial(double* xValues, double* yValues, int n, double _x) {
	double lagrangePol = 0.0;
	for (int i = 0; i < n; i++) {
		double P = 1.0;

		for (int j = 0; j < n; j++)
			if (j != i) {
				double a = xValues[i];
				double b = xValues[j];

				__asm {
					finit;

					fld _x;
					fld b;
					fsub;

					fld a;
					fld b;
					fsub;

					fdiv;

					fld P;
					fmul;
					fst P;
				}
			}

		double a = yValues[i];
		__asm {
			finit;

			fld P;
			fld a;
			fmul;

			fld lagrangePol;
			fadd;
			fst lagrangePol;
		}
	}

	return lagrangePol;
}