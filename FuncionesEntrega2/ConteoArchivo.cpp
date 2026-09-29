#include "ConteoArchivo.h"
#include "ConteoTexto.h" // Incluimos tu lógica avanzada
#include <iostream>
#include <string>

using namespace std;

void ConteoArchivo() {
    string ruta;
    cout << "\n===== CONTEO SOBRE ARCHIVO =====\n";
    cout << "Ingrese la ruta del archivo a analizar (0 para volver): ";
    getline(cin >> ws, ruta);

    if (ruta == "0") return;

    ResultadoConteo resultado;
    string error;

    // Llamamos a TU función avanzada que maneja el UTF-8 a la perfección
    if (contarArchivo(ruta, resultado, error)) {
        cout << "\n----- Resumen de conteo -----" << endl;
        cout << "Vocales:               " << resultado.vocales << endl;
        cout << "Consonantes:           " << resultado.consonantes << endl;
        cout << "Caracteres especiales: " << resultado.especiales << endl;
        cout << "Palabras:              " << resultado.palabras << endl;
    } else {
        cout << "Error: " << error << endl;
    }

    cout << "\n1) para Volver : ";
    int opcion;
    cin >> opcion;
}
