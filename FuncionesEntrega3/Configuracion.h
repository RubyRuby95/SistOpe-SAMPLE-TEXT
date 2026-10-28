// ============================================================================
// ITEM 1.2 - ARCHIVO DE SALIDA (y variables de entorno propias, item 3)
// El nombre del archivo del indice NO se escribe en el codigo: viene de una variable de entorno.
//
// Variables que lee este modulo:
//   INDEX_FILE      archivo de salida del indice (ej. indice_invertido.idx)   -> item 1.2
//   BOOKS_MAP_FILE  archivo con la asociacion "ID;nombre del libro"           -> item 1.9
//
// Otras variables las lee el companero con leerVariable():
//   LIBROS_DIR (item 1.3) y N_THREADS (item 1.6)
// ============================================================================
#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <string>

struct Configuracion {
  std::string archivoIndice;   // valor de INDEX_FILE
  std::string archivoMapa;     // valor de BOOKS_MAP_FILE
};

// Lee una variable: primero del entorno real (getenv) y, si no esta, del archivo .env.
// Devuelve "" si no existe. Es publica para que el companero la use con LIBROS_DIR y N_THREADS.
std::string leerVariable(const std::string& nombre);

// Valida que INDEX_FILE y BOOKS_MAP_FILE existan (item 3.4).
// Si falta alguna devuelve false y deja el motivo en "error".
bool cargarConfiguracion(Configuracion& cfg, std::string& error);

#endif
