#include "../Headers/CargarUsuarios.h"

using namespace std;

void CargarUsuarios(vector<Usuario>& listaUsuarios){
  string nombreArchivo = obtenerVariableEnv("USER_FILE");
    if (nombreArchivo.empty()) {
        cout << "Error: no se encontro la variable USER_FILE.\n";
        return;
    }
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Aviso: no se encontro " << nombreArchivo
             << " (puede que aun no haya usuarios).\n";
        return;
    }
    string linea;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        stringstream ss(linea);
        string campo;
        Usuario u;
        
        try {
            getline(ss, campo, ';'); 
            u.id = stoi(campo);
        } catch (const exception& e) {
            cout << "Error al leer el ID del usuario. Saltando linea defectuosa.\n";
            continue; 
        }
        getline(ss, campo, ';'); u.nombre = campo;
        getline(ss, campo, ';'); u.username = campo;
        getline(ss, campo, ';'); u.password = campo;
        getline(ss, campo, ';'); u.perfil = campo;
        listaUsuarios.push_back(u);
    }
    archivo.close();
  
}
