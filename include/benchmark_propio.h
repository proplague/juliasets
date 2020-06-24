#ifndef __BENCHMARK_PROP__
#define __BENCHMARK_PROP__
#include <chrono>
#include <iostream>
#include <fstream>
#include "../include/img.h"
using namespace std::chrono;

//defino mis funciones benchmark
int benchmark_transformar(size_t lim_inf, size_t lim_sup, size_t saltos_pixeles, size_t iteraciones, function_t ffunction, ostream *file);
#endif