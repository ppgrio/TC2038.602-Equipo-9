#include <array>
#include <numeric>

#define BREAK __asm__("int $3")

#define ARRAY_SIZE 200

int main()
{
    std::array<int, ARRAY_SIZE> int_array;

    std::iota(int_array.begin(), int_array.end(), 1);

    return 0;
}
