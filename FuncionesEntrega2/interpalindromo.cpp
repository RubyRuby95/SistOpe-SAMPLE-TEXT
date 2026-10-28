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
    int SoN;
    cout << "Desea continuar (1 para continuar, 0 para volver): " << endl;
    cin >> SoN;

    if(SoN == 0) return;
    cout << "Ingrese un texto: ";
    cin >> texto;

    while (true){
        cout <<"\n===== OPCIONES =====\n";
        cout << "1. Validar si el texto es palíndromo" << endl;
        cout << "2. Cancelar" << endl;
        int opcion;
        cin >> opcion;
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "Entrada inválida. Por favor, ingrese un número." << endl;
            continue; 
        }
        
        if (opcion == 2) {
            cout << "Operación cancelada." << std::endl;
            return;
        }
        if (opcion != 1 && opcion != 2) {
            cout << "Opción inválida." << std::endl;
        }
        cout << endl;

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