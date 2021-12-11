from ctypes import *


class LagrangeInterpolator:
    def __init__(self, lib):
        self.lib = lib

    def lagrange_interpolate(self, x_values, y_values, initial_count, start, end, dots_count, x_buffer, y_buffer):
        x = start
        deltaX = (end - start) / (dots_count - 1)
        for i in range(dots_count):
            x_buffer.append(x)
            y_buffer.append(
                self.interpolate_lagrange_polynomial(
                    x_values,
                    y_values,
                    initial_count,
                    x
                )
            )
            x += deltaX
        return 1

    def interpolate_lagrange_polynomial(self, x_values, y_values, n, _x):
        lagrangePol = 0.0
        for i in range(n):
            P = 1.0
            for j in range(n):
                if j != 1:
                    a = x_values[i]
                    b = x_values[j]
                    P = self.lib.InterpolateLagrangePolynomialFirsASM(
                        c_double(_x),
                        c_double(a),
                        c_double(b),
                        c_double(P)
                    )

            a = y_values[i]
            lagrangePol = self.lib.InterpolateLagrangePolynomialSecondASM(
                c_double(P),
                c_double(a),
                c_double(lagrangePol)
            )
        return lagrangePol