#include <array>
#include <numeric>

int main()
{
    std::array<int, 200> int_array;

    std::iota(int_array.begin(), int_array.end(), 1);

    return 0;
}
