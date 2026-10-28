// ============================================================================
// ITEM 1.11 - FINALIZACION DEL PROCESAMIENTO
// Controla que el programa termine bien: sin libros pendientes en la cola,
// con todos los libros procesados, con todos los threads terminados
// y con el indice completo.
//
// Tambien define "Progreso": el contador (protegido con semaforo) donde cada
// thread del companero (item 1.5) anota si termino bien o mal cada libro.
// ============================================================================
#ifndef FINALIZACION_H
#define FINALIZACION_H

#include "ConstruccionIndice.h"
#include "IdentificacionLibros.h"
#include "Semaforos.h"

#include <string>
#include <vector>

// Avance del procesamiento. Lo comparten todos los threads, por eso lleva un semaforo.
class Progreso {
public:
  explicit Progreso(std::size_t totalLibros) : total(totalLibros), mutex(1) {}

  // El thread llama a este metodo cuando termino un libro correctamente.
  void registrarExito(const Libro& libro, std::size_t palabrasDistintas);
  // El thread llama a este metodo cuando no pudo procesar un libro (con el motivo).
  void registrarFallo(const Libro& libro, const std::string& motivo);

  // Se consultan despues del join, cuando ya no hay threads corriendo.
  int procesados() const { return nProcesados; }
  int fallidos() const { return nFallidos; }
  const std::vector<std::string>& errores() const { return listaErrores; }

private:
  std::size_t total;                       // total de libros a procesar
  int nProcesados = 0;                     // libros procesados correctamente
  int nFallidos = 0;                       // libros que dieron error
  std::vector<std::string> listaErrores;   // mensajes de los libros con error
  Semaforo mutex;                          // protege los contadores y la salida por pantalla
};

// Crea la cola y los threads, reparte los libros, espera a que todos terminen (join)
// y verifica que no quedo trabajo pendiente.
// Devuelve true solo si todo termino correctamente; si no, deja el motivo en "error".
bool ejecutarProcesamiento(const std::vector<Libro>& libros, int nThreads,
                           IndiceGlobal& indice, Progreso& progreso, std::string& error);

// Muestra el resumen final (libros procesados, palabras, tiempo y libros con problemas).
void mostrarResumen(const std::vector<Libro>& libros, const IndiceGlobal& indice,
                    const Progreso& progreso, double segundos, const std::string& archivoIndice);

#endif
