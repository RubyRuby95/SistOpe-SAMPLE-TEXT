#include "../Headers/CargarPerfiles.h"

using namespace std;


void CargarPerfiles(std::vector<Perfil>& listaPerfiles){

  string nombreArchivo = obtenerVariableEnv("PERFIL_FILE");
    if (nombreArchivo.empty()) {
        cout << "Error: no se encontro la variable PERFIL_FILE.\n";
        return;
    }
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Aviso: no se encontro " << nombreArchivo
             << " (puede que aun no haya perfiles).\n";
        return;
    }
    string linea;
     while (getline(archivo, linea)) {
          if (linea.empty()) continue;
          
          stringstream ss(linea);
          string campoNombre;
          string campoNumeros;
          
          Perfil p;

          if (getline(ss, campoNombre, ';')) {
              p.perfil = campoNombre;
          }

          if (getline(ss, campoNumeros, ';')) {
              stringstream ssNumeros(campoNumeros);
              string numeroStr;
              
              while (getline(ssNumeros, numeroStr, ',')) {
                  if (!numeroStr.empty()) {
                      p.funciones.push_back(stoi(numeroStr));
                  }
              }
          }
          
          listaPerfiles.push_back(p);
      }
      archivo.close();
}

