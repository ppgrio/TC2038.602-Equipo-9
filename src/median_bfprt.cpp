#include "../include/median_bfprt.h"
#include "../include/config.h"

#include <array>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <utility>

// Ordenamiento por inserción manual para subgrupos pequeños (<= 5 elementos)
static void insertion_sort_subgroup(std::vector<unsigned int>& arr, size_t left, size_t right) {
    for (size_t i = left + 1; i <= right; ++i) {
        unsigned int key = arr[i];
        long long j = static_cast<long long>(i) - 1;
        long long l = static_cast<long long>(left);

        while (j >= l && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

// Algoritmo de selección determinista BFPRT (Mediana de Medianas)
static unsigned int select_bfprt(std::vector<unsigned int>& a, size_t k) {
    size_t n = a.size();

    // Caso base: Para arreglos pequeños, se ordena y devuelve el k-ésimo elemento directamente
    if (n <= 5) {
        insertion_sort_subgroup(a, 0, n - 1);
        return a[k];
    }

    // 1) Dividir en grupos de 5 y encontrar la mediana de cada subgrupo
    std::vector<unsigned int> medians;
    medians.reserve((n + 4) / 5);

    for (size_t i = 0; i < n; i += 5) {
        size_t sub_right = std::min(i + 4, n - 1);
        insertion_sort_subgroup(a, i, sub_right);
        // La mediana del grupo de 5 está en el punto medio del subgrupo
        medians.push_back(a[i + (sub_right - i) / 2]);
    }

    // 2) Obtener el pivote (mediana de medianas) de manera recursiva
    unsigned int pivot = select_bfprt(medians, medians.size() / 2);

    // 3) Particionar el arreglo en 3 grupos: Menores (L), Iguales (E) y Mayores (G)
    std::vector<unsigned int> L, E, G;
    L.reserve(n / 2);
    E.reserve(1);
    G.reserve(n / 2);

    for (unsigned int x : a) {
        if (x < pivot) {
            L.push_back(x);
        } else if (x == pivot) {
            E.push_back(x);
        } else {
            G.push_back(x);
        }
    }

    // 4) Descarte de particiones según la posición de k
    if (k < L.size()) {
        return select_bfprt(L, k);
    } else if (k < L.size() + E.size()) {
        return pivot;
    } else {
        return select_bfprt(G, k - L.size() - E.size());
    }
}

unsigned int median_bfprt(std::array<unsigned int, ARRAY_SIZE> a) {
    // Convertir el std::array a std::vector para permitir redimensionamiento dinámico en la recursión
    std::vector<unsigned int> vec(a.begin(), a.end());
    size_t n = vec.size();

    if (n == 0) {
        throw std::invalid_argument("El arreglo no puede estar vacio");
    }

    // Retornar el elemento en el índice central n / 2
    return select_bfprt(vec, n / 2);
}