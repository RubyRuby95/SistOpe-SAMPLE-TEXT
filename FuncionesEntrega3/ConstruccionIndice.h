// ============================================================================
// ITEM 1.10 - CONSTRUCCION DEL INDICE INVERTIDO
// Junta los resultados parciales de todos los threads en UN unico indice.
// Cada palabra aparece una sola vez, con todos los libros donde se encontro:
//     palabra;(idLibro,veces);(idLibro,veces)...
// ============================================================================
#ifndef CONSTRUCCION_INDICE_H
#define CONSTRUCCION_INDICE_H

#include "ConteoPalabras.h"
#include "Semaforos.h"

#include <cstddef>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class IndiceGlobal {
public:
  IndiceGlobal() : mutex(1) {}     // semaforo binario: inicia libre (valor 1)

  // Lo llama cada thread cuando termina un libro: mezcla su conteo parcial con el indice global.
  // Usa un semaforo porque todos los threads escriben en el mismo indice (SECCION CRITICA).
  void integrar(int idLibro, const ConteoLibro& parcial);

  // Cantidad de palabras distintas del indice (solo para el resumen final).
  std::size_t cantidadPalabras() const { return indice.size(); }

  // Escribe el indice en "archivo" (palabras en orden alfabetico, libros por ID).
  // Solo se llama cuando todos los threads ya terminaron.
  bool guardar(const std::string& archivo, std::string& error);

private:
  // palabra -> lista de pares (ID del libro, veces que aparece en ese libro)
  std::unordered_map<std::string, std::vector<std::pair<int, long long>>> indice;
  Semaforo mutex;                  // protege "indice"
};

#endif
