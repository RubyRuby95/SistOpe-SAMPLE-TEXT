#include "ConstruccionIndice.h"

#include <algorithm>   // sort
#include <fstream>

using namespace std;

void IndiceGlobal::integrar(int idLibro, const ConteoLibro& parcial){
  Guardia g(mutex);                  // SECCION CRITICA: solo un thread a la vez modifica el indice
  for (const auto& par : parcial){   // par.first = palabra, par.second = veces en este libro
    indice[par.first].emplace_back(idLibro, par.second);   // agrega (libro, veces) a esa palabra
  }
}                                    // al salir de la funcion, la Guardia libera el semaforo

bool IndiceGlobal::guardar(const string& archivo, string& error){
  // Los threads terminan en cualquier orden, asi que el orden se impone aqui al escribir.
  // Eso hace que el archivo sea igual con 1 thread o con 20.
  vector<const string*> palabras;
  palabras.reserve(indice.size());
  for (const auto& par : indice) palabras.push_back(&par.first);   // junta todas las palabras
  sort(palabras.begin(), palabras.end(),
       [](const string* a, const string* b){ return *a < *b; });   // orden alfabetico

  ofstream salida(archivo, ios::trunc);          // crea (o vacia) el archivo de salida
  if (!salida.is_open()){
    error = "No se pudo crear el archivo de indice \"" + archivo + "\".";
    return false;
  }
  for (const string* p : palabras){
    auto& entradas = indice[*p];                 // lista (libro, veces) de esta palabra
    sort(entradas.begin(), entradas.end());      // ordena por ID de libro
    salida << *p;                                // escribe la palabra
    for (const auto& e : entradas){
      salida << ";(" << e.first << ',' << e.second << ')';   // ;(ID,veces)
    }
    salida << '\n';                              // una linea por palabra
  }
  salida.flush();                                // fuerza la escritura a disco
  if (!salida){
    error = "Error al escribir \"" + archivo + "\".";
    return false;
  }
  return true;
}
