import os
from flask import Flask, jsonify, request  
import matplotlib

matplotlib.use("Agg")

import matplotlib.pyplot as plt
import numpy as np

# Inicializar la aplicación Flask (LÍNEA CORREGIDA)
app = Flask(__name__)

# Crear carpeta de imágenes automáticamente
os.makedirs("../imagenes", exist_ok=True)


@app.route("/upload", methods=["POST"])
def recibir():
    try:
        # Leer JSON recibido
        datos = request.json

        # Obtener identificador
        capture_id = datos["capture_id"]

        # Convertir lista a matriz NumPy
        matriz = np.array(datos["data"])

        print("\n==============================")
        print(f"Recibida captura: {capture_id}")
        print(f"Dimensiones: {matriz.shape}")

        # Crear figura
        plt.figure(figsize=(6, 6))

        # Generar mapa táctil según los requisitos
        plt.imshow(matriz, cmap="inferno")
        plt.colorbar(label="Presion")
        plt.title(f"Mapa tactil {capture_id}")

        # Nombre del archivo para guardar en la ruta correcta
        nombre = f"../imagenes/capture_{capture_id}.png"

        # Guardar imagen
        plt.savefig(nombre)

        # Liberar memoria
        plt.close()

        print(f"Imagen guardada: {nombre}")
        print("==============================")

        return jsonify({"estado": "ok"})

    except Exception as e:
        print("\nERROR DETECTADO")
        print(str(e))

        return jsonify({"estado": "error", "mensaje": str(e)}), 500


if __name__ == "__main__":
    # Arranca el servidor local
    app.run(host="0.0.0.0", port=5000)
