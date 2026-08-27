#include <iostream>
#include "../Headers/ListarPerfiles.h"
#include "../Headers/Config.h"
#include "../Headers/EliminarPerfiles.h"
using namespace std;

//Funcion local usada para buscar 
int buscarPerfil(string perfil, vector<Perfil>& listaPerfiles){
  size_t i = 0;
  while(i< listaPerfiles.size() && listaPerfiles[i].perfil != perfil){
    i++;
  }
  if(i >= listaPerfiles.size()) return -1;
  return i;
}


void eliminarPerfiles(vector<Perfil>& listaPerfiles) {
    ListarPerfiles(listaPerfiles, false);
    string nombreArchivo = obtenerVariableEnv("PERFIL_FILE");
    if (nombreArchivo.empty()) {
        cout << "Error: no se encontro la variable PERFIL_FILE.\n";
        return;
    }
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Aviso: no se encontro " << nombreArchivo
             << " (puede que aun no haya usuarios).\n";
        return;
    }
    
    string nombrePerfil;
    cout << "Ingrese nombre de perfil a eliminar: ";
    cin >> nombrePerfil;
    cout << endl;
    
    int indice = buscarPerfil(nombrePerfil, listaPerfiles);
    if(indice != -1){
        listaPerfiles.erase(listaPerfiles.begin() + indice);
        
        ofstream archivoSalida(nombreArchivo, ios::trunc);
        
        if (!archivoSalida.is_open()) {
      cout << "Error: No se pudo abrir el archivo para guardar los cambios.\n";
      return;
      }
     for (size_t i = 0; i < listaPerfiles.size(); i++) {
        // 1. Escribes el nombre y el punto y coma (usa el atributo correcto, ej: .nombre)
        archivoSalida << listaPerfiles[i].perfil << ";";
        
        // 2. Ejecutas un bucle independiente para los permisos
        for(size_t j = 0; j < listaPerfiles[i].funciones.size(); j++) {
            archivoSalida << listaPerfiles[i].funciones[j];
            
            // Evita poner una coma después del último número
            if (j < listaPerfiles[i].funciones.size() - 1) {
                archivoSalida << ",";
            }
        }
        
        // 3. Imprimes el salto de línea al final de cada perfil
        archivoSalida << "\n";
    }

    archivoSalida.close();
    cout << "Archivo actualizado exitosamente." << endl;
    }
    else{
      cout << "Perfil no existe..." << endl;
    }   
    cout << "ejecutando la función eliminarUsuario..." << endl;
    cout << endl;
}
