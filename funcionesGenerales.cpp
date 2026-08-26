#include "funcionesGenerales.h"
using namespace std;

string cambiaraMayuscula(string texto){
  for(char& letra : texto){
    letra = toupper(letra);
  }
  return texto;
}

string convertiraMinuscula(string texto){
  for (char& letra : texto) {
        letra = tolower(letra);
  }
  return texto;
}

