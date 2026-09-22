#include "../include/median_quickselect.h"

#include "../include/config.h"

#include <array>
#include <cstddef>
#include <random>
#include <stdexcept>
#include <utility>

std::array<unsigned int, ARRAY_SIZE> lomuto_partition(
    std::array<unsigned int, ARRAY_SIZE> &arr,
    size_t low,
    size_t high,
    int &i_return)
{
    if (low == 0 and high == 0)
    {
        low = 0;
        high = arr.size() - 1;
    }

    unsigned int pivot = arr[high];

    long long int i = low - 1;
    for (int j = low; j < high; ++j)
    {
        if (arr[j] <= pivot)
        {
            i += 1;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);

    i_return = (i + 1);
    return arr;
}

unsigned int quickselect(
    std::array<unsigned int, ARRAY_SIZE> &a,
    size_t k)
{
    if (k >= a.size())
    {
        throw std::out_of_range("k fuera de rango");
    }

    size_t lo = 0;
    size_t hi = a.size() - 1;

    while (true)
    {
        if (lo == hi)
        {
            return a[lo];
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dist(lo, hi);
        size_t p = dist(gen);

        std::swap(a[p], a[hi]);

        int i = 0;
        a = lomuto_partition(a, lo, hi, i);

        if (k == i)
        {
            return a[i];
        }

        if (k < i)
        {
            hi = i - 1;
        }
        else
        {
            lo = i + 1;
        }
    }
}
unsigned int median_quickselect(std::array<unsigned int, ARRAY_SIZE> &a)
{
    std::array<unsigned int, ARRAY_SIZE> b = a;

    size_t n = a.size();
    if (n % 2)
    {
        return quickselect(b, n / 2);
    }

    unsigned int lo = quickselect(a, n / 2 - 1);

    std::array<unsigned int, ARRAY_SIZE> c = a;
    unsigned int hi = quickselect(c, n / 2);

    return (lo + hi) / 2;
}
