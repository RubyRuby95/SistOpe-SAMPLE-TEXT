#include "interpalindromo.h"

#include <limits>

using namespace std;

bool esPalindromo(std::string texto) {
    int longitud = texto.length();
    for (int i = 0; i < longitud / 2; i++) {
        if (texto[i] != texto[longitud - i - 1]) {
            return false;
        }
    }
    return true;
}

void interpalindromo() {
    string texto;
    cout << "Ingrese un texto: ";
    cin >> texto;

    while (true){
        cout <<"\n===== OPCIONES =====\n";
        cout << "1. Validar si el texto es palíndromo" << endl;
        cout << "2. Cancelar" << endl;
        int opcion;
        cin >> opcion;
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Entrada inválida. Por favor, ingrese un número." << endl;
            continue; // Prompt the user again
        }
        
        if (opcion == 2) {
            cout << "Operación cancelada." << std::endl;
            return;
        }
        if (opcion != 1 && opcion != 2) {
            cout << "Opción inválida." << std::endl;
        }
        if (opcion == 1) {
            cout << "Verificando si el texto es palindromo..." << std::endl;
            if (esPalindromo(texto)) {
                cout << "El texto es un palíndromo." << std::endl;
                break;
            } else {
                cout << "El texto no es un palíndromo." << std::endl;
                break;
            }
        }
        
    }
}