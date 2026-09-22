#include "../include/median_sort.h"

#include "../include/config.h"

#include <array>
#include <cstdint>
#include <vector>

constexpr size_t RADIX = 256;
constexpr size_t INSERTION_SORT_THRESHOLD = 16;

void insertion_sort(
    std::array<unsigned int, ARRAY_SIZE> &arr,
    size_t left,
    size_t right)
{
    for (size_t i = left + 1; i < right; ++i)
    {
        unsigned int temp = arr[i];
        size_t j = i;

        while (j > left && arr[j - 1] > temp)
        {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = temp;
    }
}

void msd_radix_sort(
    std::array<unsigned int, ARRAY_SIZE> &arr,
    std::vector<unsigned int> &aux,
    size_t left,
    size_t right,
    int shift)
{
    if (right - left <= 1 || shift < 0)
    {
        return;
    }

    if (right - left <= INSERTION_SORT_THRESHOLD)
    {
        insertion_sort(arr, left, right);
        return;
    }

    size_t count[RADIX + 1] = {0};
    for (size_t i = left; i < right; ++i)
    {
        uint8_t byte = (arr[i] >> shift) & 0xFF;
        count[byte + 1]++;
    }

    for (size_t r = 0; r < RADIX; ++r)
    {
        count[r + 1] += count[r];
    }

    size_t bucket_starts[RADIX + 1];
    for (size_t r = 0; r <= RADIX; ++r)
    {
        bucket_starts[r] = left + count[r];
    }

    size_t current_pos[RADIX];
    for (size_t r = 0; r < RADIX; ++r)
    {
        current_pos[r] = bucket_starts[r];
    }

    for (size_t i = left; i < right; ++i)
    {
        uint8_t byte = (arr[i] >> shift) & 0xFF;
        aux[current_pos[byte]++] = arr[i];
    }

    for (size_t i = left; i < right; ++i)
    {
        arr[i] = aux[i];
    }

    for (size_t r = 0; r < RADIX; ++r)
    {
        size_t b_left = bucket_starts[r];
        size_t b_right = bucket_starts[r + 1];
        if (b_right > b_left)
        {
            msd_radix_sort(arr, aux, b_left, b_right, shift - 8);
        }
    }
}

unsigned int median_via_sort(std::array<unsigned int, ARRAY_SIZE> arr)
{
    std::vector<unsigned int> aux(ARRAY_SIZE);

    constexpr int initial_shift = (sizeof(unsigned int) - 1) * 8;
    msd_radix_sort(arr, aux, 0, ARRAY_SIZE, initial_shift);

    return arr[ARRAY_SIZE / 2 - 1];
}
