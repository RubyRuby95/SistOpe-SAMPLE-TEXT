#include "ColaTrabajo.h"
void ColaTrabajo::encolar(int valor) {
  {
    Guardia lock(mutex_);
    cola_.push(valor);
  }
  disponibles_.post();
}
int ColaTrabajo::desencolar() {
  disponibles_.wait();
  Guardia lock(mutex_);
  int valor = cola_.front();
  cola_.pop();
  return valor;
}
bool ColaTrabajo::vacia() {
  Guardia lock(mutex_);
  return cola_.empty();
}
