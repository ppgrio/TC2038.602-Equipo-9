// Benchmark harness para el reporte de TC2038.602 (Ejercicio 1).
// Corre las tres implementaciones 5 veces sobre el mismo arreglo barajado
// y mide SOLO el tiempo de calculo de la mediana.
// Uso: compilar junto con src/median_*.cpp definiendo ARRAY_SIZE en include/config.h
#include "include/median_bfprt.h"
#include "include/median_quickselect.h"
#include "include/median_sort.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <memory>
#include <numeric>
#include <random>

namespace
{

template <typename Func>
double time_median_ms(Func func, unsigned int &out_median)
{
    const auto start = std::chrono::steady_clock::now();
    out_median = func();
    const auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

} // namespace

int main()
{
    constexpr std::size_t kRuns = 5;

    // El arreglo vive en el heap: 4 MB no caben comodos en el stack de Windows.
    auto data = std::make_unique<std::array<unsigned int, ARRAY_SIZE>>();
    std::iota(data->begin(), data->end(), 0U);

    std::random_device rd;
    std::mt19937 gen(rd());

    std::cout << "size,algorithm,run,median_ms,median_value\n";
    for (std::size_t run = 0; run < kRuns; ++run)
    {
        std::shuffle(data->begin(), data->end(), gen);

        {
            const auto input = *data;
            unsigned int median = 0;
            const double ms =
                time_median_ms([&] { return median_via_sort(input); }, median);
            std::cout << ARRAY_SIZE << ",sort," << run << ',' << std::fixed
                      << std::setprecision(4) << ms << ',' << median << '\n';
        }
        {
            auto input = *data; // median_quickselect toma referencia no-const
            unsigned int median = 0;
            const double ms =
                time_median_ms([&] { return median_quickselect(input); }, median);
            std::cout << ARRAY_SIZE << ",quickselect," << run << ',' << std::fixed
                      << std::setprecision(4) << ms << ',' << median << '\n';
        }
        {
            const auto input = *data;
            unsigned int median = 0;
            const double ms =
                time_median_ms([&] { return median_bfprt(input); }, median);
            std::cout << ARRAY_SIZE << ",bfprt," << run << ',' << std::fixed
                      << std::setprecision(4) << ms << ',' << median << '\n';
        }
    }

    return EXIT_SUCCESS;
}
