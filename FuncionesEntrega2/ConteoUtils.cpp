#include "ConteoUtils.h"
#include <cctype>

using namespace std;

static bool esVocal(char c) {
    c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

ResultadoConteo realizarConteo(istream& entrada) {
    ResultadoConteo r;
    char c;
    bool dentroDePalabra = false;

    while (entrada.get(c)) {
        unsigned char uc = static_cast<unsigned char>(c);

        if (isalpha(uc)) {
            if (esVocal(c)) r.vocales++;
            else r.consonantes++;
            dentroDePalabra = true;

        } else if (isspace(uc)) {
            if (dentroDePalabra) {
                r.palabras++;
                dentroDePalabra = false;
            }

        } else {
            // números, signos de puntuación, símbolos, etc.
            r.especiales++;
            if (dentroDePalabra) {
                r.palabras++;
                dentroDePalabra = false;
            }
        }
    }

    // Si el archivo no termina con espacio/salto de línea, contar la última palabra
    if (dentroDePalabra) {
        r.palabras++;
    }

    return r;
}

void mostrarConteo(const ResultadoConteo& resultado) {
    cout << "\n===== RESUMEN DE CONTEO =====\n";
    cout << "Vocales: " << resultado.vocales << "\n";
    cout << "Consonantes: " << resultado.consonantes << "\n";
    cout << "Caracteres especiales: " << resultado.especiales << "\n";
    cout << "Palabras: " << resultado.palabras << "\n";
}
