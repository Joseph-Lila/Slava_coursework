#pragma once

double Teilor_function_value(double x_arg, double* arg, double* fun, int arr_size, double** derivation, int derivation_order);
long double fact(int N);
int find_nearest_index(double value, double* arr, int arr_size);
double** calculate_all_derivation_with_extreme(double* arg, double* fun, int arr_size, double h, int max_derrivative_order);
double left_final_derivate(double* fun, double h);
double right_final_derivate(double* fun, double h, int fun_size);