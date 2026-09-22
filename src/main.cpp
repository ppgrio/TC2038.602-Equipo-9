#include "../include/config.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <random>

void print_array(std::array<int, ARRAY_SIZE> &arr)
{
    std::cout << "arr: [";
    for (int number : arr)
    {
        std::cout << number << ' ';
    }
    std::cout << "\b \b]\n";
}

int main()
{
    std::array<int, ARRAY_SIZE> int_array;

    std::iota(int_array.begin(), int_array.end(), 1);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(int_array.begin(), int_array.end(), gen);

    print_array(int_array);

    return 0;
}
