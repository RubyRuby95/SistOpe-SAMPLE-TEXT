#include "IndiceInvertido.h"
#include "../funcionesGenerales.h"      // verificarNumero(): lee un entero validado

// ---- Modulos propios ----
#include "Configuracion.h"              // 1.2  archivo de salida (INDEX_FILE, BOOKS_MAP_FILE)
#include "IdentificacionLibros.h"       // 1.9  ID de cada libro
#include "ConstruccionIndice.h"         // 1.10 indice global
#include "Finalizacion.h"               // 1.11 termino del procesamiento

// ---- Modulos del COMPANERO (ver INTERFAZ_COMPANEROS.md) ----
#include "CarpetaLibros.h"              // 1.3  carpeta de libros
#include "NumeroThreads.h"              // 1.6  variable N_THREADS

#include <chrono>                       // para medir el tiempo
#include <iostream>

using namespace std;

void CrearIndiceInvertido(){
  cout << "\n===== CREAR INDICE INVERTIDO =====" << endl;
  string error;   // aqui cada paso deja el motivo si falla

  // Paso 1 (1.2): leer del entorno el nombre del archivo de salida
  Configuracion cfg;
  if (!cargarConfiguracion(cfg, error)){ cout << "[!] " << error << endl; return; }

  // Paso 2 (1.6, companero): leer y validar N_THREADS
  int nThreads = 0;
  if (!leerNThreads(nThreads, error)){ cout << "[!] " << error << endl; return; }

  // Paso 3 (1.3, companero): verificar la carpeta y obtener las rutas de los libros
  vector<string> rutas;
  if (!listarArchivosTxt(rutas, error)){ cout << "[!] " << error << endl; return; }

  // Paso 4 (1.9): asignar un ID a cada libro y guardar el archivo "ID;nombre"
  vector<Libro> libros = asignarIds(rutas);
  if (!guardarMapaLibros(cfg.archivoMapa, libros, error)){ cout << "[!] " << error << endl; return; }

  cout << "Libros encontrados: " << libros.size() << endl;
  cout << "Threads: " << nThreads << endl;
  cout << "Archivo de indice: " << cfg.archivoIndice << endl;
  cout << "Archivo de IDs:    " << cfg.archivoMapa << endl << endl;

  // Paso 5 (1.10 y 1.11): objetos compartidos por todos los threads
  IndiceGlobal indice;               // indice invertido que se va llenando
  Progreso progreso(libros.size());  // contador de libros procesados / con error

  // Paso 6 (1.11): crear threads, repartir libros y esperar a que terminen
  auto inicio = chrono::steady_clock::now();
  bool completo = ejecutarProcesamiento(libros, nThreads, indice, progreso, error);
  double segundos = chrono::duration<double>(chrono::steady_clock::now() - inicio).count();

  if (!completo){
    cout << "[!] " << error << " No se genero el indice." << endl;
  }
  // Paso 7 (1.2 y 1.10): escribir el indice en el archivo de INDEX_FILE
  else if (!indice.guardar(cfg.archivoIndice, error)){
    cout << "[!] " << error << endl;
  }
  else {
    mostrarResumen(libros, indice, progreso, segundos, cfg.archivoIndice);   // 1.11
  }

  // Espera a que el usuario vuelva al menu principal
  cout << "\n0) Volver" << endl;
  cout << "Opcion: ";
  while (verificarNumero() != 0){
    cout << "Opcion invalida, ingrese 0 para volver." << endl;
    cout << "Opcion: ";
  }
}
