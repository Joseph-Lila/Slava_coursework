from Operations import fact
from ctypes import *


class TeilorInterpolator:
    def __init__(self, lib):
        self.lib = lib

    def teilor_interpolate(self, x_values, y_values, initial_count, start, end, dots_count, x_buffer, y_buffer):
        h = (x_values[initial_count - 1] - x_values[0]) / (initial_count - 1)
        count_of_derivative = 5
        derivation = self.calculate_all_derivation_with_extreme(x_values, y_values, initial_count, h,count_of_derivative)
        x = start
        deltaX = (end - start) / (dots_count - 1)
        for i in range(len(dots_count)):
            x_buffer[i] = x
            y_buffer[i] = self.teilor_function_value(x, x_values, y_values, initial_count, derivation,count_of_derivative)
            x += deltaX
        return 1


    def calculate_all_derivation_with_extreme(self, arg, fun, arr_size, h, max_derrivative_order):
        der = [[], []]
        for item in der:
            for i in range(arr_size):
                item.append(0)

    def left_final_derivate(self, fun, h):
        k = 0
        res =-fun[k + 2] + 4 * fun[k + 1] - 3 * fun[k]
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
            res = self.lib.MethodTeilorASM(
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
        difference = self.lib.MethodNearestFirstASM(
            c_double(value),
            c_double(arr[0]),
            c_double(arr_size)
        )
        for i in range(arr_size):
            item = arr[i]
            index = self.lib.MethodNearestSecondASM(
                c_double(value),
                c_double(item),
                c_double(difference),
                c_int(i),
                c_int(index)
            )
        return index

    def calculate_error(self, input_str, str_len, start_x, finish_x, amount_x, fun):
        exact_value = []
        self.lib.calculateExpressionInRange(input_str, str_len, start_x, finish_x, amount_x, exact_value)
        error = 0
        for i in range(len(amount_x)):
            error += abs(exact_value[i] - fun[i])
        return error / amount_x


if __name__ == "__main__":
    TeilorInterpolator(None).calculate_all_derivation_with_extreme(1, 2, 5, 1, 1)