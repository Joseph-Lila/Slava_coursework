from unittest import *
from ctypes import *
from numpy.ctypeslib import ndpointer
import numpy as np


def fact(n):
    if n < 0:
        return 0
    if n == 0:
        return 1
    return n * fact(n - 1)


class Testing(TestCase):
    def setUp(self) -> None:
        self.lib = CDLL(r"D:\University\Slava_coursework\New\dll\math.dll")

    def test_my_function(self):
        my_function = self.lib.my_function
        my_function.restype = c_double
        assert 4 == my_function()

    def test_calculate_expression(self):
        calculate_expression = self.lib.calculateExpression
        calculate_expression.argtypes = [c_char_p, c_int, c_double]
        calculate_expression.restype = c_double
        assert calculate_expression(b"sin(x)", len("sin(x)"), 0) == 0

    def test_MethodNearestFirstASM(self):
        method_nearest_firstASM = self.lib.MethodNearestFirstASM
        method_nearest_firstASM.argtypes = [c_double, c_double, c_double]
        method_nearest_firstASM.restype = c_double
        assert 5 == method_nearest_firstASM(6, 1, 0)
        assert 5 == method_nearest_firstASM(1, 6, 10)

    def test_MethodTeilorASM(self):
        method_nearest_firstASM = self.lib.MethodTeilorASM
        method_nearest_firstASM.argtypes = [c_double, c_double, c_double, c_double]
        method_nearest_firstASM.restype = c_double
        assert 8 == method_nearest_firstASM(6, 1, 1, 2)
        assert 5 == method_nearest_firstASM(6, 2, 1, 2)

    def test_MethodNearestSecondASM(self):
        method_nearest_firstASM = self.lib.MethodNearestSecondASM
        method_nearest_firstASM.argtypes = [ndpointer(c_double), c_int, c_double, c_double, c_int]
        method_nearest_firstASM.restype = c_int
        arr = np.array([11, 12, 10.2], dtype=c_double)
        value = 10
        difference = 0.5
        assert 2 == method_nearest_firstASM(arr, len(arr), value, difference, 0)

    def test_fact(self):
        assert 120 == fact(5)
