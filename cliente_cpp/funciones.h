#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <vector>
#include <string>

using namespace std;

// Lee las capturas desde el JSON
vector<vector<vector<double>>> leerJSON(
    const string& archivo
);

// Comprueba que sea una matriz 16x16
bool validarMatriz(
    const vector<vector<double>>& matriz
);

// Interpolación bilineal manual
vector<vector<double>> interpolarBilineal(
    const vector<vector<double>>& matriz,
    int nuevoAncho,
    int nuevoAlto
);

// Envía la matriz al servidor Flask
bool enviarHTTP(
    int id,
    const vector<vector<double>>& matriz
);

#endif
