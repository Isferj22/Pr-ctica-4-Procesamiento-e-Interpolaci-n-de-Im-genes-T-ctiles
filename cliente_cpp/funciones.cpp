#include "funciones.h"

#include <iostream>
#include <fstream>
#include <cmath>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;


// =====================================
// Leer JSON
// =====================================
vector<vector<vector<double>>> leerJSON(
    const string& archivo
)
{
    ifstream fichero(archivo);

    json datos;

    fichero >> datos;

    vector<vector<vector<double>>> capturas;

    for(auto& captura : datos["captures"])
    {
        capturas.push_back(
            captura["matrix"]
            .get<vector<vector<double>>>()
        );
    }

    return capturas;
}


// =====================================
// Validar matriz 16x16
// =====================================
bool validarMatriz(
    const vector<vector<double>>& matriz
)
{
    if(matriz.size() != 16)
        return false;

    for(auto& fila : matriz)
    {
        if(fila.size() != 16)
            return false;
    }

    return true;
}


// =====================================
// Interpolación bilineal manual
// =====================================
vector<vector<double>> interpolarBilineal(
    const vector<vector<double>>& matriz,
    int nuevoAncho,
    int nuevoAlto
)
{
    vector<vector<double>> resultado(
        nuevoAlto,
        vector<double>(nuevoAncho)
    );

    double escalaX =
        (double)(matriz[0].size()-1)
        /(nuevoAncho-1);

    double escalaY =
        (double)(matriz.size()-1)
        /(nuevoAlto-1);

    for(int y=0; y<nuevoAlto; y++)
    {
        for(int x=0; x<nuevoAncho; x++)
        {
            double gx = x * escalaX;
            double gy = y * escalaY;

            int x1 = floor(gx);
            int y1 = floor(gy);

            int x2 = min(
                x1+1,
                (int)matriz[0].size()-1
            );

            int y2 = min(
                y1+1,
                (int)matriz.size()-1
            );

            double dx = gx - x1;
            double dy = gy - y1;

            double q11 = matriz[y1][x1];
            double q21 = matriz[y1][x2];
            double q12 = matriz[y2][x1];
            double q22 = matriz[y2][x2];

            resultado[y][x] =
                q11*(1-dx)*(1-dy)
                +
                q21*dx*(1-dy)
                +
                q12*(1-dx)*dy
                +
                q22*dx*dy;
        }
    }

    return resultado;
}


// =====================================
// Envío HTTP
// =====================================
bool enviarHTTP(
    int id,
    const vector<vector<double>>& matriz
)
{
    CURL* curl = curl_easy_init();

    if(!curl)
        return false;

    json datos;

    datos["capture_id"] = id;
    datos["width"] = 128;
    datos["height"] = 128;
    datos["data"] = matriz;

    string mensaje = datos.dump();

    struct curl_slist* headers = NULL;

    headers = curl_slist_append(
        headers,
        "Content-Type: application/json"
    );

    curl_easy_setopt(
        curl,
        CURLOPT_URL,
        "http://127.0.0.1:5000/upload"
    );

    curl_easy_setopt(
        curl,
        CURLOPT_HTTPHEADER,
        headers
    );

    curl_easy_setopt(
        curl,
        CURLOPT_POSTFIELDS,
        mensaje.c_str()
    );

    CURLcode res =
        curl_easy_perform(curl);

    curl_easy_cleanup(curl);

    return res == CURLE_OK;
}
