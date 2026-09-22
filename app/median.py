"""Mediana de precios de una lista de productos usando partición de Lomuto."""
from random import randint


def lomuto_partition(arr, low, high):
    """Partición de Lomuto: usa arr[high] como pivote."""
    pivot = arr[high]
    i = low - 1
    for j in range(low, high):
        if arr[j] <= pivot:
            i += 1
            arr[i], arr[j] = arr[j], arr[i]
    arr[i + 1], arr[high] = arr[high], arr[i + 1]
    return i + 1


def quickselect(a, k):
    """k-ésimo menor elemento (0-indexado) usando partición de Lomuto."""
    lo, hi = 0, len(a) - 1
    while True:
        if lo == hi:
            return a[lo]
        p = randint(lo, hi)
        a[p], a[hi] = a[hi], a[p]
        i = lomuto_partition(a, lo, hi)
        if k == i:
            return a[i]
        elif k < i:
            hi = i - 1
        else:
            lo = i + 1


def mediana_precio(productos):
    """Mediana de los precios usando quickselect (partición de Lomuto)."""
    precios = [p["precio"] for p in productos]
    n = len(precios)
    if n == 0:
        return None
    if n % 2:
        return quickselect(precios, n // 2)
    lo = quickselect(precios[:], n // 2 - 1)
    hi = quickselect(precios[:], n // 2)
    return (lo + hi) / 2


def analizar(productos):
    """Devuelve la mediana, los productos en la posición mediana y el listado ordenado."""
    m = mediana_precio(productos)
    ordenados = sorted(productos, key=lambda p: p["precio"])
    n = len(ordenados)
    if n % 2:
        indices = [n // 2]
    else:
        indices = [n // 2 - 1, n // 2]
    return m, [ordenados[i] for i in indices], ordenados, indices
