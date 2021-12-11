class LagrangeInterpolator:
    def __init__(self, lib):
        self.lib = lib

    def lagrange_interpolate(self, x_values, y_values, initial_count, start, end, dots_count, x_buffer, y_buffer):
        x = start
        deltaX = (end - start) / (dots_count - 1)
        for i in range(len(dots_count)):
            x_buffer[i] = x
            y_buffer[i] = self.lib.InterpolateLagrangePolynomial(x_values, y_values, initial_count, x)
            x += deltaX
        return 1


    def interpolate_lagrange_polynomial(self, x_values, y_values, n, _x):
        lagrangePol = 0.0
        for i in range(n):
            P = 1.0
            for j in range(n):
                if j != 1 :
                    a = x_values[i]
                    b = x_values[j]
                    self.lib.InterpolateLagrangePolynomialFirsASM

            a = y_values[i]
            self.lib.InterpolateLagrangePolynomialSecondASM

        return  lagrangePol