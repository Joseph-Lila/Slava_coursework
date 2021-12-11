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
            expr,
            len(expr),
            start,
            finish,
            size_initial,
            y_initial
        )

    # GUI!!!
    if y_initial is None:
        print("Выражение некорректно записано")
        return

    print("--Y--")
    for item in y_initial:
        print(str(item) + "\n", end="")
    print("-----", end="\n")
    print("\n\n")

    size_interpolate = 100

    x_teilor_interpolate = []
    y_teilor_interpolate = []

    x_teilor_interpolate, y_teilor_interpolate = TeilorInterpolator(lib).\
        teilor_interpolate(
        x_initial,
        y_initial,
        size_initial,
        start,
        finish,
        size_interpolate,
        x_teilor_interpolate,
        y_teilor_interpolate
    )

    print("-----X-----")
    for i in range(size_interpolate):
        print(x_teilor_interpolate[i])
    print("-----------")
    print("-----Y-----")
    for i in range(size_interpolate):
        print(y_teilor_interpolate[i])
    print("-----------")
    error = TeilorInterpolator(lib).\
        calculate_error(
        expr,
        len(expr),
        start,
        finish,
        size_interpolate,
        y_teilor_interpolate
    )
    print("error = ", error)


if __name__ == "__main__":
    main()
