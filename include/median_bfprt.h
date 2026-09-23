#ifndef MEDIAN_BFPRT_H
#define MEDIAN_BFPRT_H

#include "config.h"
#include <array>
#include <cstddef>

// Función principal para obtener la mediana mediante BFPRT
unsigned int median_bfprt(std::array<unsigned int, ARRAY_SIZE> a);

#endif // MEDIAN_BFPRT_H