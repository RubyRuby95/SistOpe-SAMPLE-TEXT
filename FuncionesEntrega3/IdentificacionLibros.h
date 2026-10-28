// ============================================================================
// ITEM 1.9 - IDENTIFICACION DE LOS LIBROS
// Cada libro recibe un ID numerico unico. El indice usa el ID en vez del nombre,
// y la asociacion ID <-> nombre se guarda en un archivo (BOOKS_MAP_FILE).
// ============================================================================
#ifndef IDENTIFICACION_LIBROS_H
#define IDENTIFICACION_LIBROS_H

#include <string>
#include <vector>

// Datos de un libro. Esta estructura la usan tambien los modulos del companero.
struct Libro {
  int id;               // identificador numerico unico (1, 2, 3...)
  std::string nombre;   // solo el nombre del archivo (ej. Heidi.txt)
  std::string ruta;     // ruta completa para poder abrirlo
};

// Recibe las rutas de los libros y les asigna ID 1, 2, 3... en el mismo orden recibido.
// Si las rutas vienen ordenadas, los ID son los mismos en cada ejecucion.
std::vector<Libro> asignarIds(const std::vector<std::string>& rutas);

// Guarda en "archivo" una linea "ID;nombre" por libro. Devuelve false y "error" si falla.
bool guardarMapaLibros(const std::string& archivo, const std::vector<Libro>& libros, std::string& error);

#endif
