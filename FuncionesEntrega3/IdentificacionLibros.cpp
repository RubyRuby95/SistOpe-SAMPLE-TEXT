#include "IdentificacionLibros.h"

#include <filesystem>   // para sacar el nombre del archivo desde la ruta
#include <fstream>      // ofstream

using namespace std;

vector<Libro> asignarIds(const vector<string>& rutas){
  vector<Libro> libros;
  int id = 1;                                    // el primer libro tiene ID 1
  for (const auto& ruta : rutas){
    // ID actual (y luego se incrementa), nombre = ultima parte de la ruta, y la ruta completa
    libros.push_back({id++, filesystem::path(ruta).filename().string(), ruta});
  }
  return libros;
}

bool guardarMapaLibros(const string& archivo, const vector<Libro>& libros, string& error){
  ofstream salida(archivo, ios::trunc);          // crea el archivo (o lo vacia si ya existia)
  if (!salida.is_open()){
    error = "No se pudo crear el archivo de mapeo \"" + archivo + "\".";
    return false;
  }
  for (const auto& l : libros){
    salida << l.id << ';' << l.nombre << '\n';   // formato pedido: 1;nombre.txt
  }
  salida.flush();                                // fuerza la escritura a disco
  if (!salida){                                  // comprueba que no hubo error de escritura
    error = "Error al escribir \"" + archivo + "\".";
    return false;
  }
  return true;
}
