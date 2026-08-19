#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "ListarUsuario.h"
#include "Config.h"
using namespace std;

static bool usuariosCargados = false;
static bool cargarUsuariosDesdeArchivo(vector<Usuario>& listaUsuarios) {

    string nombreArchivo = obtenerVariableEnv("USER_FILE");
    if (nombreArchivo.empty()) {
        cout << "Error: no se encontro la variable USER_FILE.\n";
        return false;
    }
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Aviso: no se encontro " << nombreArchivo
             << " (puede que aun no haya usuarios).\n";
        return false;
    }
    listaUsuarios.clear();
    string linea;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        stringstream ss(linea);
        string campo;
        Usuario u;
        getline(ss, campo, ';'); u.id = stoi(campo);
        getline(ss, campo, ';'); u.nombre = campo;
        getline(ss, campo, ';'); u.username = campo;
        getline(ss, campo, ';'); u.password = campo;
        getline(ss, campo, ';'); u.perfil = campo;
        listaUsuarios.push_back(u);
    }
    archivo.close();
    return true;
}

void listarUsuario(vector<Usuario>& listaUsuarios) {
    if (!usuariosCargados) {
        cargarUsuariosDesdeArchivo(listaUsuarios);
        usuariosCargados = true;
    }
    cout << "\nLista de usuarios\n";
    cout << "Id\tNombre\t\tPerfil\n";
    if (listaUsuarios.empty()) {
        cout << "(No hay usuarios registrados)\n";
    } else {
        for (const Usuario& u : listaUsuarios) {
            cout << u.id << "\t" << u.nombre << "\t\t" << u.perfil << "\n";
        }
    }
    cout << "\n1) para Volver : ";
    int opcion;
    cin >> opcion;
}
