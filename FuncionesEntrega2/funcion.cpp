#include "funcion.h"
#include <iostream>
#include <limits>
using namespace std;

void mainfuncion() {
    int opcion;

    while (true) {
        cout << "\n--- CALCULO DE f(x) ---" << endl;
        cout << "1. Indicar X y calcular f(x) = x*x + 2x + 8" << endl;
        cout << "0. VOLVER" << endl;
        cout << "Ingrese opcion: ";
        cin >> opcion;

        if (cin.fail()) { //Comprueba si la entrada fue inválida.
            cout << "Entrada invalida. Ingrese una opcion numerica." << endl;
            cin.clear(); // Limpia el estado de error de cin
            cin.ignore(10000, '\n'); // elimina la entrada inválida del buffer
            continue; //ejecuta desde el inicio el while
        }

        if (opcion == 1) {
            double x;
            cout << "Ingrese el valor real de X: ";
            cin >> x;

            while (cin.fail()) {
                cout << "Entrada invalida. Por favor, ingrese un numero real." << endl;
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Ingrese el valor real de X: ";
                cin >> x;
            }

            cout << "\nf(x) = x*x + 2x + 8" << endl;
            cout << "f(" << x << ") = " << x << "*" << x << " + 2*" << x << " + 8 = " << funcion(x) << endl;
        } else if (opcion == 0) {
            cout << "Volviendo al menu principal..." << endl;
            return;
        } else {
            cout << "Opcion no valida. Intente de nuevo." << endl;
        }
    }
}

double funcion(double x) {
    return x * x + 2 * x + 8;
}