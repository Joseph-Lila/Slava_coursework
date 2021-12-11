#pragma once

//double* LagrangeInterpolate(double* xValues, double* yValues, double start, double end, int dotsCount);
double InterpolateLagrangePolynomial(double* xValues, double* yValues, int n, double _x);