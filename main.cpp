using namespace std;
#include <iostream>
#include "CrearUsuario.cpp"
#include "ListarUsuario.cpp"
#include "EliminarUsuario.cpp"

struct Usuario{
    int id;
    string nombre;
    string username;
    string password;
    string perfil;
};
int main() {
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
            crearUsuario();
            break;
        case 2:
            cout << "Listando usuarios..." << endl;
            listarUsuarios();
            break;
        case 3:
            cout << "Eliminando usuario..." << endl;
            eliminarUsuario();
            break;
        default:
            cout << "Opción inválida" << endl;
    }
}