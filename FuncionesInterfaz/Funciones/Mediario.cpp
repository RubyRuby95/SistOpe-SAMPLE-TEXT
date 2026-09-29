#include "../Headers/Mediario.h"

using namespace std;

void mediario() {
    vector<Usuario> listaUsuarios;
    vector<Perfil> listaPerfiles;
    CargarUsuarios(listaUsuarios);
    CargarPerfiles(listaPerfiles);
    int opcion;
    
    while (true) {
        
        cout << "\n===== MENÚ =====\n";
        
        cout << "0. Salir" << endl;        
        cout << "1. Gestionar Usuarios" << endl;
        cout << "2. Gestionar Perfiles" << endl;
        cout << "Opcion: ";
        opcion = verificarNumero();
        
        switch (opcion) {
            case 0:
                cout << "Saliendo del programa..." << endl;
                return;
            case 1:
                UserManager(listaUsuarios, listaPerfiles);
                break;
            case 2:
                ProfileManager(listaUsuarios, listaPerfiles);
                break;
            default:
                cout << "Opción inválida" << endl;
        }
    } while (opcion != 0);
    return;
}
