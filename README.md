# Procesamiento e Interpolación de Imágenes Táctiles para un Gripper Robótico

## 1. Introducción
Este proyecto implementa un sistema distribuido para mejorar la percepción táctil en robótica. Permite que un brazo robótico manipule objetos frágiles con precisión.

## 2. Descripción del Problema
Los sensores táctiles de bajo coste ofrecen resoluciones muy bajas (matrices de \(16 \times 16\)). Esto limita la cantidad de detalle disponible para la manipulación. 
La aplicación cliente-servidor resuelve esto mediante:
1. Validación y procesamiento de datos táctiles en C++.
2. Escalado manual por interpolación bilineal a \(128 \times 128\).
3. Transmisión de datos interpolados mediante HTTP POST.
4. Reconstrucción y renderizado de mapas térmicos de presión en Python.

## 3. Estructura del Proyecto
El repositorio se organiza de la siguiente manera:
```text
trabajo_4/
├── cliente_cpp/          # Código fuente del cliente en C++
│   ├── main.cpp          # Punto de entrada principal
│   ├── funciones.cpp     # Implementación de la lógica táctil e HTTP
│   └── funciones.h       # Declaración de funciones y cabeceras
├── servidor/             # Código fuente del servidor en Python
│   └── servido.py        # Servidor API Flask
└── imagenes/             # Almacenamiento automático de mapas (.png)
```

## 4. Requisitos del Sistema
Para compilar y ejecutar este proyecto localmente se necesita:

### Cliente C++
* Compilador G++ compatible con C++11 o superior.
* Librería `libcurl` (peticiones HTTP).
* Librería `nlohmann/json` (procesamiento de JSON).

### Servidor Python
* Python 3.8 o superior.
* Dependencias instaladas: `Flask`, `NumPy` y `Matplotlib`.

## 5. Instalación

### En sistemas basados en Ubuntu/Debian:
```bash
# Actualizar los paquetes del sistema
sudo apt update

# Instalar dependencias para el cliente C++
sudo apt install g++ libcurl4-openssl-dev nlohmann-json3-dev

# Instalar dependencias para el servidor Python
pip install flask numpy matplotlib
```

## 6. Ejecución

### Paso 1: Iniciar el servidor Python
Entra en la carpeta del servidor y arranca el script:
```bash
cd servidor
python3 servido.py
```
*El servidor se iniciará en `http://127.0.0.1:5000` esperando peticiones.*

### Paso 2: Compilar y ejecutar el cliente C++
Abre una nueva terminal, compila los módulos y ejecuta el programa principal:
```bash
cd cliente_cpp
g++ -std=c++11 main.cpp funciones.cpp -o cliente -lcurl
./cliente
```

## 7. Funcionamiento
1. **Lectura**: El archivo `funciones.cpp` carga 50 capturas desde `tactile_data.json`.
2. **Validación**: Comprueba estrictamente que las dimensiones de cada matriz sean de $16 \times 16$.
3. **Interpolación**: Multiplica la resolución por un factor de escala de 8x mediante un algoritmo bilineal manual.
4. **Envío**: Envía los datos mediante `HTTP POST` en formato JSON a la ruta `/upload` del servidor.
5. **Renderizado**: El servidor `servido.py` procesa la matriz con NumPy y genera un gráfico con Matplotlib.

## 8. Resultado Esperado
El servidor Python generará automáticamente 50 mapas táctiles en formato `.png` dentro del directorio `../imagenes/`. Las imágenes usarán la escala de color `inferno` e incluirán barras de intensidad de presión.

## 9. Autor
Ismael Fernandez Jorreto
