#include "pch.h"
#include "lagrange_interpolator.h"
#include "my_library.h"

/// <summary>
/// ���������� �������� ��������������� �������
/// </summary>
/// <param name="xValues">������ �������� x �������� �������</param>
/// <param name="yValues">������ �������� � �������� �������</param>
/// <param name="initialCount">������ �������� �������� �������� �������</param>
/// <param name="start">��������� �������� x ��������������� �������</param>
/// <param name="end">�������� �������� x ��������������� �������</param>
/// <param name="dotsCount">��������� ���������� �������� ��������������� �������</param>
/// <returns>������ ����������.</returns>
int LagrangeInterpolate(double* xValues, double* yValues, int initialCount, double start, double end, int dotsCount, double* xBuffer, double* yBuffer) {
	//������ ���� ��������, �� �� ������� �� ���������� ��������.
	/*int j = 0;
	for (double x = start; x < end; x += (end - start) / (dotsCount - 1)) {
		xBuffer[j] = x;
		yBuffer[j] = InterpolateLagrangePolynomial(xValues, yValues, initialCount, x);
		j++;
	}*/

	// ������� �� ���������� ��������.
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
/// ���������� �������� ��������������� ������� ��� ������� �������� ��������� _x � �������� �������.
/// </summary>
/// <param name="xValues">������ �������� x �������� �������</param>
/// <param name="yValues">������ �������� y �������� �������</param>
/// <param name="n">������� ��������</param>
/// <param name="_x">�������� ��������� x</param>
/// <returns>�������� ��������������� ������� ��� ����������� _x</returns>
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