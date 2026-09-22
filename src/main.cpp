#include <array>
#include <numeric>

#define BREAK __asm__("int $3")

int main()
{
    std::array<int, 200> int_array;

    std::iota(int_array.begin(), int_array.end(), 1);

    return 0;
}
