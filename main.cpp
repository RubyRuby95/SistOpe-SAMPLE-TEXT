#include <iostream>
#include "CrearUsuario.h"
#include "ListarUsuario.h"
#include "EliminarUsuario.h"

using namespace std;

int main() {

    vector<Usuario> listaUsuarios;

    cout << "0. Salir " << endl;
    cout << "1. Crear usuario " << endl;
    cout << "2. Listar usuarios " << endl;
    cout << "3. Eliminar usuario " << endl;
    int opcion;
    cin >> opcion;
    switch (opcion) {
        case 0:
            cout << "Saliendo del programa..." << endl;
            break;
        case 1:
            cout << "Creando usuario..." << endl;
            crearUsuario(listaUsuarios);
            break;
        case 2:
            cout << "Listando usuarios..." << endl;
            listarUsuario();
            break;
        case 3:
            cout << "Eliminando usuario..." << endl;
            eliminarUsuario();
            break;
        default:
            cout << "Opción inválida" << endl;
    }
}