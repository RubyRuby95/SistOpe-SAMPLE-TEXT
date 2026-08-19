#include <iostream>
#include <vector>
#include "CrearUsuario.h"
#include "ListarUsuario.h"
#include "EliminarUsuario.h"
using namespace std;

int main() {
    vector<Usuario> listaUsuarios;
    int opcion;
    string user = "";
    string password = "";
    while (user != "admin" && password != "1234") {
        cout << "Ingrese usuario y contraseña (usuario: admin, contraseña: 1234):" << endl;
        cout << "user: ";
        cin >> user;
        cout << "password: ";
        cin >> password;
    }
    do {
        cout << "\n===== MENÚ =====\n";
        cout << "0. Salir " << endl;
        cout << "1. Crear usuario " << endl;
        cout << "2. Listar usuarios " << endl;
        cout << "3. Eliminar usuario " << endl;
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
                listarUsuario(listaUsuarios);
                break;
            case 3:
                cout << "Eliminando usuario..." << endl;
                eliminarUsuario();
                break;
            default:
                cout << "Opción inválida" << endl;
        }
    } while (opcion != 0);
    return 0;
}
