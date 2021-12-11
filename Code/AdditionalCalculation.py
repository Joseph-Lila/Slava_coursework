from ctypes import *


class AdditionalCalculation:
    def __init__(self, lib):
        self.lib = lib

    def calculate_expression_in_range(self, input_str, str_len, start_x, finish_x, amount_x, container):
        calculate_expression = self.lib.calculateExpression
        calculate_expression.argtypes = [c_char_p, c_int, c_double]
        calculate_expression.restype = c_double
        temp_x = start_x
        delta_x = (finish_x - start_x) / (amount_x - 1)
        for i in range(amount_x):
            container.append(calculate_expression(input_str, str_len, temp_x))
            temp_x += delta_x
        return container