from AdditionalCalculation import AdditionalCalculation
from ctypes import *
from numpy.ctypeslib import ndpointer
import numpy as np


def fact(n):
    if n < 0:
        return 0
    if n == 0:
        return 1
    return n * fact(n - 1)


class TeilorInterpolator:
    def __init__(self, lib):
        self.lib = lib

    def teilor_interpolate(self, x_values, y_values, initial_count, start, end, dots_count, x_buffer, y_buffer):
        h = (x_values[initial_count - 1] - x_values[0]) / (initial_count - 1)
        count_of_derivative = 5
        derivation = self.calculate_all_derivation_with_extreme(
            x_values,
            y_values,
            initial_count,
            h,
            count_of_derivative
        )
        x = start
        deltaX = (end - start) / (dots_count - 1)
        for i in range(dots_count):
            x_buffer.append(x)
            y_buffer.append(
                self.teilor_function_value(
                    x,
                    x_values,
                    y_values,
                    initial_count,
                    derivation,
                    count_of_derivative
                )
            )
            x += deltaX
        return x_buffer, y_buffer

    def calculate_all_derivation_with_extreme(self, arg, fun, arr_size, h, max_derrivative_order):
        der = [[] for i in range(arr_size)]
        for item in der:
            for i in range(arr_size):
                item.append(0)
        der[0][0] = self.left_final_derivate(fun, h)
        for i in range(1, arr_size - 1):
            der[i][0] = (fun[i + 1] - fun[i - 1]) / (arg[i + 1] - arg[i - 1])
        der[arr_size - 1][0] = self.right_final_derivate(fun, h, arr_size)

        for k in range(1, max_derrivative_order):
            der_temp = []
            for j in range(arr_size):
                der_temp.append(der[j][k - 1])
            der[0][k] = self.left_final_derivate(der_temp, h)
            for i in range(1, arr_size - 1):
                der[i][k] = (der[i + 1][k - 1] - der[i - 1][k - 1]) / (arg[i + 1] - arg[i - 1])
            der[arr_size - 1][k] = self.right_final_derivate(der_temp, h, arr_size)
        return der

    def left_final_derivate(self, fun, h):
        k = 0
        res = -fun[k + 2] + 4 * fun[k + 1] - 3 * fun[k]
        return res / (2 * h)

    def right_final_derivate(self, fun, h, fun_size):
        k = fun_size - 1
        res = 3 * fun[k] - 4 * fun[k - 1] + fun[k - 2]
        return res / (2 * h)

    def teilor_function_value(self, x_arg, arg, fun, arr_size, derivation, derivation_order):
        nearest_index = self.find_nearesr_index(x_arg, arg, arr_size)
        res = fun[nearest_index]

        for i in range(derivation_order):
            dif = pow(x_arg - arg[nearest_index], i + 1)
            factorial = fact(i + 1)
            proizvodnaya = derivation[nearest_index][i]
            method_teilorASM = self.lib.MethodTeilorASM
            method_teilorASM.argtypes = [c_double, c_double, c_double, c_double]
            method_teilorASM.restype = c_double
            res = method_teilorASM(
                c_double(dif),
                c_double(factorial),
                c_double(proizvodnaya),
                c_double(res)
            )
        return res

    def find_nearesr_index(self, value, arr, arr_size):
        index = 0
        difference = 0
        arr_0 = arr[0]
        method_nearest_firstASM = self.lib.MethodNearestFirstASM
        method_nearest_firstASM.argtypes = [c_double, c_double, c_double]
        method_nearest_firstASM.restype = c_double
        difference = method_nearest_firstASM(
            c_double(value),
            c_double(arr_0),
            c_double(arr_size)
        )
        new_arr = np.array(arr, dtype=c_double)
        method_nearest_secondASM = self.lib.MethodNearestSecondASM
        method_nearest_secondASM.argtypes = [ndpointer(c_double), c_int, c_double, c_double, c_int]
        method_nearest_secondASM.restype = c_int
        index = method_nearest_secondASM(
            new_arr,
            arr_size,
            value,
            difference,
            index
        )
        return index

    def calculate_error(self, input_str, str_len, start_x, finish_x, amount_x, fun):
        exact_value = []
        exact_value = AdditionalCalculation(self.lib).\
            calculate_expression_in_range(
            input_str,
            str_len,
            start_x,
            finish_x,
            amount_x,
            exact_value
        )
        error = 0
        for i in range(amount_x):
            error += abs(exact_value[i] - fun[i])
        return error / amount_x


if __name__ == "__main__":
    TeilorInterpolator(None).calculate_all_derivation_with_extreme(1, 2, 5, 1, 1)