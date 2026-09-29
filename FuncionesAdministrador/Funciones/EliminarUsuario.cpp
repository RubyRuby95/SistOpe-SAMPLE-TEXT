#include <iostream>

#include "../Headers/Config.h"
#include "../Headers/EliminarUsuario.h"
#include "../../funcionesGenerales.h"
using namespace std;


//funcion local que busca el usuario en el vector usando el id entregado por el usuario
int buscarUsuario(int Id, vector<Usuario>& listaUsuarios){
  int i = 0;
  while(i< listaUsuarios.size() && listaUsuarios[i].id != Id){
    i++;
  }
  if(i >= listaUsuarios.size()) return -1;
  return i;
}

void eliminarUsuario(vector<Usuario>& listaUsuarios) {
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
    int Id = verificarNumero();
    int indice = buscarUsuario(Id, listaUsuarios);
    if(indice != -1){
        listaUsuarios.erase(listaUsuarios.begin() + indice);
        
        ofstream archivoSalida(nombreArchivo, ios::trunc);
        
        if (!archivoSalida.is_open()) {
      cout << "Error: No se pudo abrir el archivo para guardar los cambios.\n";
      return;
      }
      for (size_t i = 0; i < listaUsuarios.size(); i++) {
      archivoSalida << listaUsuarios[i].id << ";"
                  << listaUsuarios[i].nombre << ";"
                  << listaUsuarios[i].username << ";"
                  << listaUsuarios[i].password << ";"
                  << listaUsuarios[i].perfil << "\n";
    }

    archivoSalida.close();
    cout << "Archivo actualizado exitosamente." << endl;
    }
    else{
      cout << "Id de usuario no existe..." << endl;
    }   
    cout << "ejecutando la función eliminarUsuario..." << endl;
}
