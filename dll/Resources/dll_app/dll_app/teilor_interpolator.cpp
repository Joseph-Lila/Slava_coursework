#include "pch.h"
#include "teilor_interpolator.h"
#include "my_library.h"

using namespace std;

/// <summary>
/// Вычисление значений интерполирующей функции
/// </summary>
/// <param name="xValues">Массив значений x исходной функции</param>
/// <param name="yValues">Массив значений н исходной функции</param>
/// <param name="initialCount">Размер массивов значений исходной функции</param>
/// <param name="start">Начальное значение x интерполирующей функции</param>
/// <param name="end">Конечное значение x интерполирующей функции</param>
/// <param name="dotsCount">Требуемое количество значений интерполирующей функции</param>
/// <returns>Статус результата.</returns>
int TeilorInterpolate(double* xValues, double* yValues, int initialCount, double start, double end, int dotsCount, double* xBuffer, double* yBuffer) {
    // Это шаг входных данных
    double h = (xValues[initialCount - 1] - xValues[0]) / (initialCount - 1);
    int count_of_derivative = 5;
    double** derivation = calculate_all_derivation_with_extreme(xValues, yValues, initialCount, h, count_of_derivative);

    // Доходит до последнего элемента.
    double x = start;
    double deltaX = (end - start) / (dotsCount - 1);
    for (int j = 0; j < dotsCount; j++) {
        xBuffer[j] = x;
        yBuffer[j] = Teilor_function_value(x, xValues, yValues,  initialCount, derivation, count_of_derivative);
        x += deltaX;
    }
    return 1;
}


/// <summary>
/// Вычисляет n производных методом конечных разностей
/// </summary>
/// <param name="arg">Массив аргументов функции</param>
/// <param name="fun">Массив значений функции</param>
/// <param name="arr_size">Размер массива</param>
/// <param name="max_derrivative_order">Степень производной до которой считаем</param>
/// <returns>Двумерный массив с производными</returns>
double** calculate_all_derivation_with_extreme(double* arg, double* fun, int arr_size, double h, int max_derrivative_order) {
    // Формирование двумерного массива заполненного нулями.
    double** der = new double* [arr_size];
    for (int i = 0; i < arr_size; i++) {
        der[i] = new double[max_derrivative_order];
        for (int j = 0; j < max_derrivative_order; j++) {
            der[i][j] = 0;
        }
    }

    // Фишка в том что используя центральную конечную разность (на самом деле формулу Лагранжа)
    // Нельзя получить значения крайних производных (0 и n-1) для каждой степени.
    // Проблема решается применением Восходящей конечной разности для первого элемента                
    // и нисходящей конечной разности для последнего.

    // Первая производная вычисляется с использованием значения функции.
    der[0][0] = left_final_derivate(fun, h);
    for (int i = 1; i < arr_size; i++) {
        der[i][0] = (fun[i + 1] - fun[i - 1]) / (arg[i + 1] - arg[i - 1]);
    }
    der[arr_size - 1][0] = right_final_derivate(fun, h, arr_size);

    // n-я производная вычисляется с использованием n-1 производной.
    for (int k = 1; k < max_derrivative_order; k++) {
        //Выделяем временный массив из матрицы производных.
        double* der_temp = new double[arr_size];
        for (int t = 0; t < arr_size; t++) {
            der_temp[t] = der[t][k - 1];
        }

        der[0][k] = left_final_derivate(der_temp, h);
        for (int i = 1; i < arr_size - 1; i++) {
            der[i][k] = (der[i + 1][k - 1] - der[i - 1][k - 1]) / (arg[i + 1] - arg[i - 1]);
        }
        der[arr_size - 1][k] = right_final_derivate(der_temp, h, arr_size);
    }

    return der;
}

/// <summary>
/// Считаем производную для начала отрезка по трём его точкам.
/// </summary>
/// <param name="fun"></param>
/// <param name="h"></param>
/// <returns></returns>
double left_final_derivate(double* fun, double h) {
    int k = 0;
    double res = -fun[k + 2] + 4 * fun[k + 1] - 3 * fun[k];
    return res / (2 * h);
}

/// <summary>
/// Считаем производную для конца отрезка по трём точкам
/// </summary>
/// <param name="fun"></param>
/// <param name="h"></param>
/// <param name="fun_size"></param>
/// <returns></returns>
double right_final_derivate(double* fun, double h, int fun_size) {
    int k = fun_size - 1;
    double res = 3 * fun[k] - 4 * fun[k - 1] + fun[k - 2];
    return res / (2 * h);
}

/// <summary>
/// Вычисляет значение функции для заданого аргумента.
/// </summary>
/// <param name="x_arg"></param>
/// <param name="index"></param>
/// <param name="x_first_argument"></param>
/// <param name="fun"></param>
/// <param name="count"></param>
/// <param name="finite_difference"></param>
/// <returns></returns>
double Teilor_function_value(double x_arg, double* arg, double* fun, int arr_size, double** derivation, int derivation_order) {
    int nearest_index = find_nearest_index(x_arg, arg, arr_size);

    double res = fun[nearest_index];

    for (int i = 0; i < derivation_order; i++) {
        double dif = pow(x_arg - arg[nearest_index], i + 1);
        double factorial = fact(i + 1);
        double proizvodnaya = derivation[nearest_index][i];
        //double temp = (dif / factorial) * proizvodnaya;
        //res += temp;
        __asm {
            finit;

            fld dif;
            fld factorial;
            fdiv;
            fld proizvodnaya;
            fmul;
            //fst temp;
            fld res;
            fadd;
            fst res;
        }
    }

    return res;
}

/// <summary>
/// Находит индекс ближайшего элемента в массиве
/// </summary>
/// <param name="value">Значение для которого ищем ближайший элемент</param>
/// <param name="arr">Массив</param>
/// <param name="size">Размер массива</param>
/// <returns></returns>
int find_nearest_index(double value, double* arr, int arr_size) {
    int index = 0;
    //double difference = abs(value - arr[0]);
    double difference = 0;
    double arr_0 = arr[0];
    __asm {
        finit;
        fld value;
        fld arr_0;

        fsub;
        fabs;

        fst difference;
    }

    for (int i = 0; i < arr_size; i++) {
        //double temp_difference = abs(value - arr[i]);
        double item = arr[i];
        __asm {
            finit;

            fld value;
            fld item;
            fsub;
            fabs;

            fld difference;
            fxch; // обмен значений

            fcom;
            fstsw ax;
            sahf; cf = c3, pf = c2, zf = c0;
            jae _end; // if temp_difference >= difference

            fst difference;
            fld i;
            fst index;

        _end:

        }
    }
    return index;
}


//int find_nearest_index(double value, double* arr, int arr_size) {
//    int index = 0;
//    double difference = abs(value - arr[0]);
//    for (int i = 0; i < arr_size; i++) {
//        double temp_difference = abs(value - arr[i]);
//        if (temp_difference < difference) {
//            difference = temp_difference;
//            index = i;
//        }
//    }
//    return index;
//}

long double fact(int N)
{
    if (N < 0) // если пользователь ввел отрицательное число
        return 0; // возвращаем ноль
    if (N == 0) // если пользователь ввел ноль,
        return 1; // возвращаем факториал от нуля - не удивляетесь, но это 1 =)
    else // Во всех остальных случаях
        return N * fact(N - 1); // делаем рекурсию.
}


/// <summary>
/// Вычисляет погрешность приближённого метода
/// </summary>
/// <param name="inputStr">Строка с выражением</param>
/// <param name="strLen">Длина строки с выражением</param>
/// <param name="startX">Начальное значение X в промежутке</param>
/// <param name="finishX">Коненчое значение X в промежутке</param>
/// <param name="amountX">Количество значений в промежутке</param>
/// <param name="fun">Ссылка на массив уже посчитанных значений функции</param>
/// <returns>Величину погрешности</returns>
double calculate_error(char* inputStr, int strLen, double startX, double finishX, int amountX, double* fun) {
    double* exact_value = new double[amountX];
    calculateExpressionInRange(inputStr, strLen, startX, finishX, amountX, exact_value);

    double error = 0;
    for (int i = 0; i < amountX; i++) {
        error += abs(exact_value[i] - fun[i]);
    }

    return error / amountX;
}