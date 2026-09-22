#include <algorithm>
#include <array>
#include <numeric>
#include <random>

#define BREAK __asm__("int $3")

#define ARRAY_SIZE 200

int main()
{
    std::array<int, ARRAY_SIZE> int_array;

    std::iota(int_array.begin(), int_array.end(), 1);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(int_array.begin(), int_array.end(), gen);

    return 0;
}
