"""
Mini app web: encuentra la mediana de precios de una lista de productos
usando Quickselect con partición de Lomuto.
"""
import time
import random
from flask import Flask, render_template, request, jsonify

from median import mediana_precio, analizar

app = Flask(__name__)

NOMBRES = [
    "Laptop", "Mouse", "Teclado", "Monitor", "Audífonos", "Cámara",
    "Impresora", "Tablet", "Celular", "Bocina", "Cargador", "USB",
    "Silla", "Escritorio", "Lámpara", "Mochila", "Reloj", "Consola",
]


def generar_productos(n):
    return [
        {
            "nombre": f"{random.choice(NOMBRES)} {random.randint(1, 999)}",
            "precio": round(random.uniform(10, 5000), 2),
        }
        for _ in range(n)
    ]


@app.route("/")
def index():
    return render_template("index.html")


@app.route("/api/generar", methods=["POST"])
def api_generar():
    data = request.get_json() or {}
    n = int(data.get("n", 20))
    n = max(1, min(n, 5000))
    return jsonify({"productos": generar_productos(n)})


@app.route("/api/mediana", methods=["POST"])
def api_mediana():
    data = request.get_json() or {}
    productos = data.get("productos", [])

    limpios = []
    for p in productos:
        try:
            nombre = str(p.get("nombre", "")).strip()
            precio = float(p.get("precio"))
        except (TypeError, ValueError):
            continue
        if not nombre:
            continue
        limpios.append({"nombre": nombre, "precio": precio})

    if not limpios:
        return jsonify({"error": "Agrega al menos un producto con nombre y precio."}), 400

    t0 = time.perf_counter()
    mediana, en_mediana, ordenados, indices = analizar(limpios)
    t1 = time.perf_counter()

    return jsonify({
        "total": len(limpios),
        "mediana": mediana,
        "en_mediana": en_mediana,
        "indices_mediana": indices,
        "ordenados": ordenados,
        "tiempo_ms": (t1 - t0) * 1000,
    })


if __name__ == "__main__":
    app.run(debug=True, host="127.0.0.1", port=5000)
