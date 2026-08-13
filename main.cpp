using namespace std;
#include <iostream>

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
    if (opcion == 0) {
        cout << "Saliendo del programa..." << endl;
        return 0;
    }
    if (opcion == 1) {
        // Lógica para crear usuario
    }
    if (opcion == 2) {
        // Lógica para listar usuarios
    }
    if (opcion == 3) {
        // Lógica para eliminar usuario
    }
    return 0;
}