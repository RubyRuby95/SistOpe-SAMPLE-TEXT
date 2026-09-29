#include <iostream>
#include <string>
#include "../Headers/ListarUsuario.h"
#include "../Headers/Config.h"
#include "../../funcionesGenerales.h"
using namespace std;



void listarUsuario(vector<Usuario>& listaUsuarios) {
    cout << "\n===== LISTA DE USUARIOS =====\n";
    cout << "Id\tNombre\t\tCorreo\t\t\tPerfil\n";
    if (listaUsuarios.empty()) {
        cout << "(No hay usuarios registrados)\n";
    } else {
        for (const Usuario& u : listaUsuarios) {
            cout << u.id << "\t" << u.nombre << "\t\t" << u.username << "\t\t" << u.perfil << "\n";
        }
    }
    cout << "\nIngrese 1 para Volver : ";
    int opcion = verificarNumero();
    
}
