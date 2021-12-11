from ctypes import *


def calculate_expression_in_range(input_str, str_len, start_x, finish_x, amount_x, container, lib):
    calculate_expression = lib.calculateExpression
    calculate_expression.argtypes = [c_char_p, c_int, c_double]
    calculate_expression.restype = c_double
    temp_x = start_x
    delta_x = (finish_x - start_x) / (amount_x - 1)
    for i in range(amount_x):
        container.append(calculate_expression(input_str, str_len, temp_x))
        temp_x += delta_x
    return container


def teilor_interpolate(x_values, y_values, initial_count, start, end, dots_count, x_buffer, y_buffer, lib):
    calculate_all_derivation_with_extreme = lib.calculate_all_derivation_with_extreme
    calculate_all_derivation_with_extreme.argtypes = []

def main():
    pass

if __name__ == "__main__":
    main()
