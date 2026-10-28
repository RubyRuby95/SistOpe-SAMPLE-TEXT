#include "Configuracion.h"
#include "../FuncionesAdministrador/Config.h"   // obtenerVariableEnv(): lee el archivo .env

#include <cstdlib>   // getenv

using namespace std;

string leerVariable(const string& nombre){
  const char* valor = getenv(nombre.c_str());                  // 1) busca en el entorno real
  if (valor != nullptr && valor[0] != '\0') return valor;     //    si existe y no esta vacia, la usa
  return obtenerVariableEnv(nombre);                           // 2) si no, la busca en el .env
}

bool cargarConfiguracion(Configuracion& cfg, string& error){
  cfg.archivoIndice = leerVariable("INDEX_FILE");              // item 1.2: nombre del .idx
  if (cfg.archivoIndice.empty()){
    error = "La variable de entorno INDEX_FILE no esta definida.";
    return false;
  }

  cfg.archivoMapa = leerVariable("BOOKS_MAP_FILE");            // item 1.9: nombre del archivo ID;libro
  if (cfg.archivoMapa.empty()){
    error = "La variable de entorno BOOKS_MAP_FILE no esta definida.";
    return false;
  }
  return true;                                                 // todo correcto
}
