# Datos para el reporte — TC2038.602 Equipo 9 (Actividad: La mediana)

> Paquete de datos para redactar el documento de la actividad. Incluye descripción
> del código, métricas, complejidades teóricas, benchmark real (promedio y
> desviación estándar de 5 corridas), y los problemas que deben corregirse antes
> de la entrega/demo. Los gráficos y CSV están en `benchmark_results/`.

---

## 1. Datos generales

- **Lenguaje elegido:** C++17 (g++ 10.3.0, TDM-GCC-64, Windows 10, flags `-O2 -Wall -Wextra`).
- **Repositorio (privado):** https://github.com/ppgrio/TC2038.602-Equipo-9
- **Entrada de prueba:** arreglo con los enteros `0 … N-1` generados con `std::iota`,
  barajados con `std::shuffle` (mt19937). Tamaños: N = 100001, 200001, …, 1000001
  (todos impares → existe un único elemento central en la posición N/2).
- **Medición:** `std::chrono::steady_clock`, cronometrando **solo** la llamada que
  calcula la mediana (la generación y el shuffle quedan fuera). 5 corridas por
  tamaño, re-barajando entre corridas; los 3 algoritmos reciben los mismos datos.
- **Mediana esperada** (para verificar corrección): con valores `0 … N-1`, la
  mediana es exactamente **N/2** (ej. N=100001 → 50000; N=1000001 → 500000).

## 2. Descripción de las tres implementaciones

### 2.1 Ordena + Selecciona — `src/median_sort.cpp` (103 líneas)
- No usa un ordenamiento por comparación: implementa **MSD radix sort** en base
  256 (por bytes del `unsigned int`), con **insertion sort** para subarreglos de
  ≤16 elementos (`INSERTION_SORT_THRESHOLD`). Requiere arreglo auxiliar de N.
- Después de ordenar, devuelve el elemento central.
- ⚠️ **Bug de índice:** `src/median_sort.cpp:102` devuelve `arr[ARRAY_SIZE / 2 - 1]`.
  Con N impar eso es **un índice antes** del centro; debe ser `arr[ARRAY_SIZE / 2]`.
  Verificado en el benchmark: este método imprime N/2 − 1 en las 50 corridas,
  mientras los otros dos imprimen N/2.

### 2.2 Quickselect con partición de Lomuto — `src/median_quickselect.cpp` (100 líneas)
- `lomuto_partition` (línea 11): pivote = último elemento, índice `i` de menores,
  intercambios en el peor caso n−1.
- `quickselect` (línea 40): pivote **aleatorio** (`std::uniform_int_distribution`),
  itera hasta aislar el índice k = N/2; sin ordenar todo el arreglo.
- `median_quickselect` (línea 84): copia el arreglo para no destruir la entrada.
- Ineficiencias (no afectan corrección, sí constantes): crea `std::random_device`
  y `std::mt19937` **en cada ronda** de partición, y `lomuto_partition` devuelve el
  arreglo **por valor** → una copia completa de N elementos por ronda
  (`a = lomuto_partition(...)` en la línea 67).

### 2.3 Mediana de medianas (BFPRT) — `src/median_bfprt.cpp` (85 líneas)
- Selección determinista: divide en grupos de 5, ordena cada grupo con insertion
  sort manual, toma la mediana de cada grupo, y el pivote = mediana de medianas
  (recursión). Partición en 3 vías (menores L, iguales E, mayores G) con vectores,
  y descarte recursivo del lado que contiene k. Caso base n ≤ 5.
- Garantía teórica O(n) en el peor caso.

## 3. Complejidad del código (líneas y mantenibilidad)

| Módulo | Líneas (.cpp) | Funciones | Percepción de mantenimiento |
|---|---|---|---|
| `median_sort.cpp` | 103 | `insertion_sort`, `msd_radix_sort`, `median_via_sort` | El radix es el más largo y el más delicado (manejo de bytes, conteos, umbral). Una vez entendido el esquema MSD es mecánico. |
| `median_quickselect.cpp` | 100 | `lomuto_partition`, `quickselect`, `median_quickselect` | Lomuto es corto y clásico; la firma de `lomuto_partition` (devuelve el arreglo entero + índice por referencia) es confusa y penaliza rendimiento. |
| `median_bfprt.cpp` | 85 | `insertion_sort_subgroup`, `select_bfprt`, `median_bfprt` | El algoritmo conceptualmente más difícil quedó sorprendentemente compacto; el uso de `std::vector` L/E/G lo hace legible. Recursión doble (medianas + descarte) cuidarla al depurar. |
| **Total algoritmos** | **288** (+38 de `main.cpp`, +36 de headers) | | — |

Headers de 10 líneas cada uno (interfaz mínima), `config.h` fija `ARRAY_SIZE` en
tiempo de compilación (cambiar de tamaño exige recompilar; el harness `benchmark.cpp`
automatiza eso).

## 4. Tiempos asintóticos teóricos (con fuentes)

| Algoritmo | Promedio | Peor caso | Espacio extra |
|---|---|---|---|
| Ordena + selecciona (radix MSD, base 256, w=4 bytes) | Θ(n·w) = **Θ(n)** p/ enteros de 32 bits | Θ(n·w) | Θ(n) auxiliar |
| Quickselect (pivote aleatorio, Lomuto) | **Θ(n)** | **Θ(n²)** | Θ(1) (in-place) |
| BFPRT (grupos de 5) | **Θ(n)** | **Θ(n)** (garantizado) | Θ(n) por nivel (partición en 3 vías) |

Recurrencia de BFPRT: T(n) ≤ T(n/5) + T(7n/10) + Θ(n) ⇒ Θ(n), porque el pivote
deja fuera de la recursión al menos ~3n/10 − 6 elementos.
Quickselect: T(n) = T(n′) + Θ(n) con E[n′] = n/2 ⇒ Θ(n) promedio; entrada
adversa (o mala suerte continua con pivote fijo) ⇒ Θ(n²); el pivote aleatorio lo
vuelve de esperanza lineal con alta probabilidad.

**Bibliografía sugerida para citar:**
- Cormen, Leiserson, Rivest, Stein. *Introduction to Algorithms* (3ª ed.), cap. 9
  “Medians and Order Statistics” (quickselect, BFPRT) y cap. 8 (radix sort).
- Blum, Floyd, Pratt, Rivest, Tarjan (1973). “Time Bounds for Selection”.
  *Journal of Computer and System Sciences* 7(4):448–461. (BFPRT)
- Hoare (1962). “Quicksort”. *Computer Journal* 5(1):10–16. (selección por partición)
- Notas de curso indicadas en la actividad (notas1, notas2) para mediana de medianas.

## 5. Benchmark — resultados reales

Equipo: Windows 10 64 bits, g++ (TDM-GCC) 10.3.0, `-O2`, un solo proceso, 5
corridas por tamaño. **(Los números finales del reporte deberían re-correrse en
Linux, que es el ambiente de evaluación; la tendencia será la misma.)**

Promedio ± desviación estándar (ms):

| N | sort (radix) | quickselect (Lomuto) | bfprt |
|---:|---:|---:|---:|
| 100001 | 0.65 ± 0.02 | 1.21 ± 0.23 | 2.43 ± 0.19 |
| 200001 | 1.17 ± 0.08 | 2.45 ± 0.39 | 4.79 ± 0.30 |
| 300001 | 1.77 ± 0.02 | 4.01 ± 0.49 | 7.37 ± 0.10 |
| 400001 | 2.57 ± 0.51 | 4.80 ± 1.13 | 9.59 ± 0.21 |
| 500001 | 3.09 ± 0.20 | 6.70 ± 0.74 | 12.20 ± 0.25 |
| 600001 | 3.57 ± 0.02 | 7.25 ± 1.32 | 14.76 ± 0.27 |
| 700001 | 4.05 ± 0.06 | 8.69 ± 1.21 | 17.04 ± 0.27 |
| 800001 | 5.13 ± 0.46 | 9.95 ± 2.09 | 21.70 ± 4.71 |
| 900001 | 5.65 ± 0.37 | 12.45 ± 2.29 | 25.04 ± 3.09 |
| 1000001 | 6.51 ± 0.32 | 12.51 ± 1.67 | 26.82 ± 3.98 |

Verificación de corrección (50/50 corridas): quickselect y bfprt devuelven **N/2**
siempre; sort devuelve **N/2 − 1** por el bug de índice (§2.1).

### Discusión (contraste observado vs. teoría)
- **Crecimiento empírico al multiplicar N ×10** (100001 → 1000001): sort ×10.0,
  quickselect ×10.3, bfprt ×11.0 → los tres se ven **lineales** en este rango,
  consistente con la teoría (radix con w fijo y los dos selección lineales en
  promedio/peor caso).
- **Constantes:** radix sort gana con claridad (≤6.5 ms en el millón) porque no
  compara y hace 4 pasadas de distribución por byte; es el más rápido aunque su
  peor caso es el “malo” en términos de comparaciones (O(n log n) para
  comparaciones genéricas — aquí no aplica por ser radix).
- **BFPRT** cumple su linealidad pero con la constante más alta: en cada nivel de
  recursión ordena subgrupos, construye la lista de medianas y particiona en
  tres vectores con asignaciones dinámicas (L/E/G). Paga ~4× el tiempo de radix
  a cambio de la garantía de peor caso.
- **Quickselect** es ~2× radix: el número de rondas es aleatorio (pivote
  aleatorio) y cada ronda copia todo el arreglo al devolverlo por valor, lo que
  explica su **mayor varianza** (σ hasta 2.29 ms vs. ~0.3 de los otros).
- Gráficos: `benchmark_results/chart_lines.png` (líneas con barras de σ) y
  `benchmark_results/chart_bars.png` (barras agrupadas). Datos crudos:
  `benchmark_results/benchmark_runs.csv` (150 filas) y agregado en
  `benchmark_results/benchmark_summary.csv`.

## 6. Estado del repositorio (para la sección de evidencia)

- URL privada: https://github.com/ppgrio/TC2038.602-Equipo-9
- 14 commits (21–22 sep), mensajes descriptivos en inglés.
- ⚠️ **Solo un integrante aparece como autor** (`A01735632@tec.mx`). La rúbrica
  pide evidencia de que **todos** contribuyeron: los otros integrantes deben hacer
  commits (aunque sea del reporte, fixes o el Ejercicio 2) antes de la entrega.
- ⚠️ `src/median_bfprt.cpp` e `include/median_bfprt.h` **no están commiteados**
  (aparecen como untracked). Falta el commit del tercer algoritmo.
- ⚠️ El `.gitignore` contiene solo `./` (patrón inválido): no ignora nada y los
  binarios/benchmark pueden subirse por accidente.

## 7. Problemas detectados que conviene corregir antes de entregar

1. **`median_sort.cpp:102`** — off-by-one: `arr[ARRAY_SIZE / 2 - 1]` debe ser
   `arr[ARRAY_SIZE / 2]`. Es visible solo con tamaños impares (con N=200 “parece”
   bien). El profe verifica la mediana impresa → fallaría el caso de prueba.
2. **3 warnings de signo** con `-Wall -Wextra` en `src/median_quickselect.cpp`
   (líneas 26, 69, 74: comparaciones `int` vs `size_t`). La rúbrica exige compilar
   sin warnings. Cambiar `int j` → `size_t` y `i_return` a `size_t`.
3. **`main.cpp` no llama a `median_bfprt`** — la demo actual solo muestra 2 de 3
   algoritmos; el profe pide imprimir la mediana en todos los casos.
4. **`include/debug.h`** — define `BREAK` como `__asm__("int $3")` (sintaxis
   GCC/Clang, no compila en MSVC) y su guarda se llama `MACROS_H` aunque el archivo
   es `debug.h`. No se usa en ningún archivo; mejor eliminarlo o corregirlo.
5. **Constantes de quickselect** (si quieren mejorar números): reusar el generador
   mt19937 en vez de reconstruirlo por ronda y particionar in-place sin devolver
   el arreglo por valor.
6. **Re-correr el benchmark en Linux** para el documento final (ambiente oficial
   de evaluación). El harness `benchmark.cpp` quedó en la raíz para eso: hay que
   recompilar por tamaño cambiando `ARRAY_SIZE` en `include/config.h`.

## 8. Qué falta del alcance (para que la otra IA/documento lo cubra)

- **Ejercicio 2 no está en esta carpeta:** la miniaplicación con interfaz gráfica
  que usa la partición de Lomuto. (Solo existe el Ejercicio 1.)
- El documento formal redactado (este archivo es el insumo de datos).
- La invitación al profesor como colaborador del repo privado y la evidencia de
  commits frecuentes de todo el equipo (§6).
