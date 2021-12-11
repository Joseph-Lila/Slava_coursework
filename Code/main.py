from ctypes import *
from AdditionalCalculation import AdditionalCalculation
from TeilorInterpolator import TeilorInterpolator
from LagrangeInterpolator import LagrangeInterpolator


def main():
    lib = CDLL(r"D:\University\Slava_coursework\New\dll\math.dll")

    size_initial = 10
    start = 0
    finish = 4.5
    expr = b"sin(x)"
    x_initial = []
    y_initial = []
    temp = start
    for i in range(size_initial):
        x_initial.append(temp)
        temp += (finish - start) / (size_initial - 1)

    print("--X--")
    for item in x_initial:
        print(item)
    print("-----")

    y_initial = AdditionalCalculation(lib).\
        calculate_expression_in_range\
        (
            c_char_p(expr),
            len(expr),
            start,
            finish,
            size_initial,
            y_initial
        )

    print("--Y--")
    for item in y_initial:
        print(str(item) + "\n", end="")
    print("-----", end="\n")
    print("\n\n")

    size_interpolate = 100
    x_teilor_interpolate = []
    y_teilor_interpolate = []

    #


if __name__ == "__main__":
    main()
