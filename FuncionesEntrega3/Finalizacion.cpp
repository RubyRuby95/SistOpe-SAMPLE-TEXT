#include "Finalizacion.h"

// Modulos del COMPANERO que se usan aqui (ver INTERFAZ_COMPANEROS.md):
#include "ColaTrabajo.h"          // item 1.4: cola compartida
#include "ThreadsTrabajadores.h"  // item 1.5: creacion de los threads

#include <iostream>

using namespace std;

void Progreso::registrarExito(const Libro& libro, size_t palabrasDistintas){
  Guardia g(mutex);   // SECCION CRITICA: varios threads suman al contador y escriben en pantalla
  nProcesados++;
  cout << "  [" << nProcesados + nFallidos << "/" << total << "] "
       << libro.nombre << " (ID " << libro.id << "): " << palabrasDistintas
       << " palabras distintas" << endl;
}

void Progreso::registrarFallo(const Libro& libro, const string& motivo){
  Guardia g(mutex);   // SECCION CRITICA
  nFallidos++;
  listaErrores.push_back(libro.nombre + ": " + motivo);   // se guarda para informarlo al final
}

bool ejecutarProcesamiento(const vector<Libro>& libros, int nThreads,
                           IndiceGlobal& indice, Progreso& progreso, string& error){
  ColaTrabajo cola;   // cola compartida (item 1.4, del companero)

  // Se crean los threads. Quedan dormidos esperando libros en la cola.
  string aviso;
  vector<thread> workers = crearTrabajadores(nThreads, libros, cola, indice, progreso, aviso);
  if (!aviso.empty()) cout << "[!] " << aviso << endl;   // por ejemplo, si se crearon menos de los pedidos
  if (workers.empty()){                                  // sin threads no se puede continuar
    error = "No se pudo crear ningun thread.";
    return false;
  }

  // Reparto del trabajo: se encola la posicion de cada libro...
  for (size_t i = 0; i < libros.size(); i++) cola.encolar(static_cast<int>(i));
  // ...y luego una marca FIN por cada thread, para avisarle que ya no hay mas libros.
  for (size_t i = 0; i < workers.size(); i++) cola.encolar(ColaTrabajo::FIN);

  // Espera a que todos los threads terminen. El programa no sigue hasta que ninguno corra.
  for (auto& w : workers) w.join();

  // Comprobaciones finales: cola vacia y todos los libros procesados (bien o con error).
  if (!cola.vacia() || progreso.procesados() + progreso.fallidos() != static_cast<int>(libros.size())){
    error = "El procesamiento no se completo (quedaron libros sin procesar).";
    return false;
  }
  return true;   // todo termino correctamente
}

void mostrarResumen(const vector<Libro>& libros, const IndiceGlobal& indice,
                    const Progreso& progreso, double segundos, const string& archivoIndice){
  cout << "\n----- Resumen -----" << endl;
  cout << "Libros procesados:  " << progreso.procesados() << " de " << libros.size() << endl;
  cout << "Palabras en indice: " << indice.cantidadPalabras() << endl;
  cout << "Tiempo:             " << segundos << " s" << endl;
  cout << "Indice guardado en: " << archivoIndice << endl;

  if (progreso.fallidos() > 0){                          // informa los libros que no se pudieron leer
    cout << "\nLibros con problemas (" << progreso.fallidos() << "), no se incluyen en el indice:" << endl;
    for (const auto& e : progreso.errores()) cout << "  - " << e << endl;
  }
}
