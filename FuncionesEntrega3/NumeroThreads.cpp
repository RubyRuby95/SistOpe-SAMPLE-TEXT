#include "NumeroThreads.h"
#include "Configuracion.h"
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <string>

using namespace std;

bool leerNThreads(int& cantidad, string& error) {
  string valor = leerVariable("N_THREADS");
  if (valor.empty()) {
    error = "Falta N_THREADS en el entorno o en .env.";
    return false;
  }
  errno = 0;
  char* fin = nullptr;
  long n = strtol(valor.c_str(), &fin, 10);
  if (errno == ERANGE || fin == valor.c_str() || *fin != '\0' || n <= 0 || n > INT_MAX) {
    error = "N_THREADS debe ser un entero positivo (recibido: \"" + valor + "\").";
    return false;
  }
  cantidad = static_cast<int>(n);
  return true;
}
