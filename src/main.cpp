#include "../include/config.h"

#include "../include/median_quickselect.h"
#include "../include/median_sort.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <random>

void print_array(std::array<unsigned int, ARRAY_SIZE> &arr)
{
    std::cout << "arr: [";
    for (unsigned int number : arr)
    {
        std::cout << number << ' ';
    }
    std::cout << "\b \b]\n";
}

int main()
{
    std::array<unsigned int, ARRAY_SIZE> int_array;

    std::iota(int_array.begin(), int_array.end(), 1);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(int_array.begin(), int_array.end(), gen);

    print_array(int_array);

    std::cout << "median_via_sort: " << median_via_sort(int_array) << '\n';
    std::cout << "median_quickselect: " << median_quickselect(int_array) << '\n';

    return 0;
}
