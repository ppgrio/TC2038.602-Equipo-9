"""
===============================================================
 Program : 02_median.py
 Author  : Juan Manuel Ahuactzin
 Date    : 2025-08-26
 Version : 1.0
===============================================================
Description:
    Implementation of different algorithms to compute the median:
    - median_via_sort: O(n log n), sorts the array and picks the middle.
    - median_quickselect: Average-case O(n), based on Quickselect.
    - median_bfprt: Deterministic O(n) worst-case using BFPRT 
      (Median of Medians).

Usage:
    python 02_median.py

Example:
    # Generates a shuffled list of integers e.g. 1..20 and prints
    # the median according to the three algorithms.

Dependencies:
    - Python 3.x
    - Standard libraries: random

Notes:
    - Demonstrates differences between selection via sorting,
      randomized Quickselect, and the deterministic BFPRT algorithm.
    - Can be adapted to test performance on larger datasets.
===============================================================
"""

from random import randint, shuffle

def median_via_sort(a):
    """Ordena y devuelve la mediana. O(n log n)."""
    n = len(a)
    b = sorted(a)
    mid = n // 2
    if n % 2:         # impar
        return b[mid]
    else:             # par: promedio de dos centrales (ajusta a tu definición)
        return (b[mid-1] + b[mid]) / 2
    

def lomuto_partition(arr, low=None, high=None):

    if low == None and high==None:
        low = 0
        high = len(arr) - 1
    
    # pivot
    pivot = arr[high] 
    
    # Index of smaller element
    i = (low - 1)
    for j in range(low, high):
        
        # If current element is smaller than or
        # equal to pivot
        if (arr[j] <= pivot):
            
            # increment index of smaller element
            i += 1 
            arr[i], arr[j] = arr[j], arr[i]
    arr[i + 1], arr[high] = arr[high], arr[i + 1]

    return arr, (i + 1)

def quickselect(a, k):
    """Devuelve el k-ésimo (0-index) elemento más pequeño. Promedio O(n)."""
    if not 0 <= k < len(a):
        raise IndexError("k fuera de rango")
    lo, hi = 0, len(a)-1
    while True:
        if lo == hi:
            return a[lo]
        # random pivot
        p = randint(lo, hi)
        # move pivot to end
        a[p], a[hi] = a[hi], a[p]

        a, i = lomuto_partition(a, lo, hi)

        if k == i:
            return a[i]
        # Search in the left part
        elif k < i:
            hi = i - 1
        # Search in the right part
        else:
            lo = i + 1

def median_quickselect(a):
    """Mediana con quickselect (in-place). Promedio O(n)."""
    n = len(a)
    b = a[:]  # no modificar la original
    if n % 2:
        return quickselect(b, n//2)
    else:
        lo = quickselect(b, n//2 - 1)
        # ojo: quickselect altera b; para el segundo mejor usa otra copia
        c = a[:]
        hi = quickselect(c, n//2)
        return (lo + hi) / 2

# --------- Mediana de medianas (BFPRT) ---------

def _partition_around_pivot(a, pivot):
    """Particiona a en < pivot, == pivot, > pivot. Devuelve (L, E, G)."""
    L, E, G = [], [], []
    for x in a:
        (L if x < pivot else E if x == pivot else G).append(x)
    return L, E, G

def _select_bfprt(a, k):
    """Selección determinista (BFPRT). O(n) en peor caso. k es 0-index."""
    n = len(a)
    if n <= 5:
        return sorted(a)[k]
    # 1) Grupos de 5 y medianas
    groups = [a[i:i+5] for i in range(0, n, 5)]
    medians = [sorted(g)[len(g)//2] for g in groups]
    # 2) Pivote = mediana de medianas (recursivo)
    pivot = _select_bfprt(medians, len(medians)//2)
    # 3) Partición
    L, E, G = _partition_around_pivot(a, pivot)
    if k < len(L):
        return _select_bfprt(L, k)
    elif k < len(L) + len(E):
        return pivot
    else:
        return _select_bfprt(G, k - len(L) - len(E))

def median_bfprt(a):
    """Mediana con BFPRT. O(n) en peor caso."""
    n = len(a)
    if n % 2:
        return _select_bfprt(a, n//2)
    else:
        lo = _select_bfprt(a, n//2 - 1)
        hi = _select_bfprt(a, n//2)
        return (lo + hi) / 2

# ---------- Demo rápida ----------
if __name__ == "__main__":
    arr = list(range(1, 200))
    shuffle(arr)
    print("arr:", arr)
    print("median_via_sort:", median_via_sort(arr))
    print("median_quickselect:", median_quickselect(arr))
    print("median_bfprt:", median_bfprt(arr))
