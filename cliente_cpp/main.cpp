#include "funciones.h"

#include <iostream>

using namespace std;

int main()
{
    auto capturas =
        leerJSON("tactile_data.json");

    cout
        << "Capturas cargadas: "
        << capturas.size()
        << endl;

    for(int i=0; i<capturas.size(); i++)
    {
        if(!validarMatriz(capturas[i]))
        {
            cout
                << "Error en captura "
                << i
                << endl;

            continue;
        }

        cout
            << "Procesando captura "
            << i
            << endl;

        auto matriz128 =
            interpolarBilineal(
                capturas[i],
                128,
                128
            );

        enviarHTTP(
            i,
            matriz128
        );
    }

    cout << "Finalizado" << endl;

    return 0;
}
