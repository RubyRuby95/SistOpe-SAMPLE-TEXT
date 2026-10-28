#ifndef THREADS_TRABAJADORES_H
#define THREADS_TRABAJADORES_H
#include "ColaTrabajo.h"
#include "Finalizacion.h"
#include "IdentificacionLibros.h"
#include <string>
#include <thread>
#include <vector>

// Crea los workers antes de cargar la cola. Si no se puede crear alguno,
// conserva los que se crearon y entrega una advertencia.
std::vector<std::thread> crearTrabajadores(
    int nThreads, const std::vector<Libro>& libros, ColaTrabajo& cola,
    IndiceGlobal& indice, Progreso& progreso, std::string& aviso);
#endif
