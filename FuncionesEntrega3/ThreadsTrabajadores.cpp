#include "ThreadsTrabajadores.h"
#include "ConteoPalabras.h"
#include <exception>
#include <iostream>

using namespace std;

namespace {
void trabajador(const vector<Libro>& libros, ColaTrabajo& cola,
                IndiceGlobal& indice, Progreso& progreso) {
  while (true) {
    int posicion = cola.desencolar();
    if (posicion == ColaTrabajo::FIN) return;
    if (posicion < 0 || static_cast<size_t>(posicion) >= libros.size()) continue;

    const Libro& libro = libros[static_cast<size_t>(posicion)];
    ConteoLibro parcial;
    string error;
    if (!contarPalabras(libro.ruta, parcial, error)) {
      progreso.registrarFallo(libro, error);
      continue;
    }
    indice.integrar(libro.id, parcial);
    progreso.registrarExito(libro, parcial.size());
  }
}
}

vector<thread> crearTrabajadores(int nThreads, const vector<Libro>& libros,
                                 ColaTrabajo& cola, IndiceGlobal& indice,
                                 Progreso& progreso, string& aviso) {
  vector<thread> workers;
  workers.reserve(static_cast<size_t>(nThreads));
  try {
    for (int i = 0; i < nThreads; ++i) {
      workers.emplace_back(trabajador, cref(libros), ref(cola), ref(indice), ref(progreso));
    }
  } catch (const exception& e) {
    aviso = string("No fue posible crear todos los threads: ") + e.what();
  }
  return workers;
}
