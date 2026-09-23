**Manual de compilación y uso\
TC2038.602 --- Equipo 9**

*Proyecto: algoritmos para obtener la mediana de un arreglo*

# 1. Descripción del proyecto

El proyecto implementa tres estrategias para obtener la mediana de un
arreglo de enteros sin signo de tamaño fijo: (1) ordenamiento MSD Radix
Sort y selección del elemento central, (2) Quickselect con partición de
Lomuto y pivote aleatorio, y (3) selección determinista BFPRT (mediana
de medianas).

La configuración del tamaño del arreglo se encuentra en
include/config.h. Por defecto, ARRAY_SIZE está definido como 200;
cualquier cambio de esta constante requiere recompilar el programa.

# 2. Requisitos

-   Sistema con un compilador C++ compatible con C++17 (por ejemplo, g++
    o clang++).

-   Make no es necesario: el proyecto puede compilarse directamente
    desde la terminal.

-   Python 3 es opcional y solo resulta útil para procesar/generar
    resultados de benchmark; el código principal y benchmark.cpp son
    C++.

-   El repositorio contiene código fuente en src/, interfaces en
    include/ y el ejecutable de benchmark en benchmark.cpp.

# 3. Estructura relevante

  ---------------------------------------------------------------------
  Ruta                               Función
  ---------------------------------- ----------------------------------
  include/config.h                   Define ARRAY_SIZE.

  include/median_sort.h              Interfaz de median_via_sort.

  include/median_quickselect.h       Interfaz de median_quickselect.

  include/median_bfprt.h             Interfaz de median_bfprt.

  src/main.cpp                       Programa de demostración.

  src/median_sort.cpp                MSD Radix Sort + selección de la
                                     mediana.

  src/median_quickselect.cpp         Quickselect + partición de Lomuto.

  src/median_bfprt.cpp               BFPRT / mediana de medianas.

  benchmark.cpp                      Programa para medir tiempos de los
                                     tres algoritmos.

  benchmark_results/                 Resultados y gráficas de
                                     benchmarks ya realizados.
  ---------------------------------------------------------------------

# 4. Preparar el proyecto

Descomprima el archivo del proyecto y sitúese en la carpeta raíz, donde
se encuentran benchmark.cpp, src/ e include/.

Linux/macOS:

cd TC2038.602-Equipo-9-master

# 5. Compilación del programa principal

El programa de demostración actual usa src/main.cpp y muestra los
resultados de median_via_sort y median_quickselect.

Con g++:

g++ -std=c++17 -Wall -Wextra -O2 src/main.cpp src/median_sort.cpp
src/median_quickselect.cpp -Iinclude -o median_demo

Ejecutar:

./median_demo

En Windows con MinGW/TDM-GCC, el equivalente es generar median_demo.exe
y ejecutarlo como median_demo.exe.

# 6. Cambiar el tamaño del arreglo

Abra include/config.h y cambie, por ejemplo, ARRAY_SIZE 200 por el
tamaño deseado. El código usa std::array\<unsigned int, ARRAY_SIZE\>,
por lo que el tamaño queda fijado durante compilación.

#define ARRAY_SIZE 200

Después de modificarlo, vuelva a compilar. No basta con ejecutar el
binario anterior.

# 7. Compilar y ejecutar los tres algoritmos

El benchmark.cpp sí invoca los tres algoritmos. Para compilarlo, use:

g++ -std=c++17 -Wall -Wextra -O2 benchmark.cpp src/median_sort.cpp
src/median_quickselect.cpp src/median_bfprt.cpp -Iinclude -o benchmark

Ejecutar:

./benchmark

El programa imprime CSV en la salida estándar. Cada tamaño configurado
realiza 5 corridas y reporta el algoritmo, corrida, tiempo en
milisegundos y valor de la mediana.

# 8. Generar un benchmark para distintos tamaños

El benchmark no recibe ARRAY_SIZE como argumento de línea de comandos;
el valor se toma de include/config.h. Para obtener mediciones con
distintos tamaños, cambie ARRAY_SIZE, compile nuevamente y ejecute el
binario para cada tamaño.

1.  Cambiar ARRAY_SIZE en include/config.h.

2.  Compilar benchmark con los tres archivos de implementación.

3.  Ejecutar benchmark y redirigir la salida a un CSV.

4.  Repetir para cada tamaño requerido.

5.  Conservar los archivos de resultados en benchmark_results/.

./benchmark \> benchmark_200.csv

# 9. Interpretación de los resultados

La columna median_ms representa el tiempo de cálculo de la mediana
medido con std::chrono::steady_clock. median_value permite verificar que
los algoritmos están produciendo el resultado esperado.

En el repositorio se incluyen resultados previos en
benchmark_results/benchmark_runs.csv y
benchmark_results/benchmark_summary.csv, además de dos gráficas.

# 10. Diferencias importantes entre las implementaciones

  ----------------------------------------------------------------------------
  Algoritmo       Archivo                      Complejidad     Observación
                                               esperada        
  --------------- ---------------------------- --------------- ---------------
  Radix Sort +    src/median_sort.cpp          Θ(n) con        Ordena el
  selección                                    enteros de 32   arreglo y toma
                                               bits            el elemento
                                                               central.

  Quickselect     src/median_quickselect.cpp   Θ(n) promedio;  Usa pivote
                                               Θ(n²) peor caso aleatorio y
                                                               partición de
                                                               Lomuto.

  BFPRT           src/median_bfprt.cpp         Θ(n) peor caso  Usa grupos de 5
                                                               y mediana de
                                                               medianas.
  ----------------------------------------------------------------------------

# 11. Problemas conocidos del estado actual del código

Las siguientes observaciones corresponden al código incluido en el
archivo entregado y deben tenerse presentes antes de usarlo como versión
final:

-   median_sort.cpp devuelve arr\[ARRAY_SIZE / 2 - 1\]. Para el concepto
    de mediana implementado por los otros algoritmos, el índice central
    usado es ARRAY_SIZE / 2; por tanto, el resultado de sort debe
    revisarse/corregirse antes de una entrega final.

-   median_quickselect.cpp contiene conversiones/comparaciones entre int
    y size_t que pueden producir warnings con -Wall -Wextra.

-   src/main.cpp no invoca median_bfprt; para una demostración de los
    tres algoritmos hay que añadir su encabezado y llamada.

-   include/debug.h no es utilizado por el programa y define BREAK con
    sintaxis de ensamblador GCC/Clang.

-   Quickselect crea un random_device y un mt19937 durante cada
    partición y la función lomuto_partition devuelve el arreglo por
    valor; esto afecta el rendimiento.

-   Los benchmarks incluidos fueron obtenidos en un entorno
    Windows/TDM-GCC y el propio reporte recomienda volver a ejecutarlos
    en Linux para una evaluación final.

# 12. Flujo recomendado para una ejecución limpia

6.  Verificar o ajustar ARRAY_SIZE en include/config.h.

7.  Compilar con -std=c++17 -Wall -Wextra -O2.

8.  Resolver cualquier warning antes de considerar la compilación como
    final.

9.  Ejecutar el programa de demostración para validar resultados.

10. Compilar benchmark.cpp junto con las tres implementaciones.

11. Ejecutar las mediciones y conservar la salida CSV.

12. Comparar median_value con un valor esperado para comprobar
    corrección.

13. Solo después de validar resultados, actualizar las gráficas y el
    reporte.

# 13. Ejemplo completo

Desde la raíz del proyecto:

g++ -std=c++17 -Wall -Wextra -O2 src/main.cpp src/median_sort.cpp
src/median_quickselect.cpp -Iinclude -o median_demo

./median_demo

g++ -std=c++17 -Wall -Wextra -O2 benchmark.cpp src/median_sort.cpp
src/median_quickselect.cpp src/median_bfprt.cpp -Iinclude -o benchmark

./benchmark \> benchmark_output.csv

# 14. Nota sobre el ejercicio 2

El archivo entregado corresponde al alcance del Ejercicio 1. El material
revisado indica que la miniaplicación con interfaz gráfica asociada al
Ejercicio 2 no se encuentra en esta carpeta; por lo tanto, este manual
cubre únicamente la compilación y uso del código presente en el
proyecto.

# 15. Resumen de comandos

  ---------------------------------------------------------------------
  Acción                             Comando
  ---------------------------------- ----------------------------------
  Compilar demo                      g++ -std=c++17 -Wall -Wextra -O2
                                     src/main.cpp src/median_sort.cpp
                                     src/median_quickselect.cpp
                                     -Iinclude -o median_demo

  Ejecutar demo                      ./median_demo

  Compilar benchmark                 g++ -std=c++17 -Wall -Wextra -O2
                                     benchmark.cpp src/median_sort.cpp
                                     src/median_quickselect.cpp
                                     src/median_bfprt.cpp -Iinclude -o
                                     benchmark

  Ejecutar benchmark                 ./benchmark

  Guardar benchmark                  ./benchmark \>
                                     benchmark_output.csv
  ---------------------------------------------------------------------

*Documento generado a partir de la estructura y código contenidos en el
proyecto entregado.*
