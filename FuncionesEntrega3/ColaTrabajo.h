#ifndef COLA_TRABAJO_H
#define COLA_TRABAJO_H
#include "Semaforos.h"
#include <queue>

// Cola FIFO compartida. Cada elemento es el indice de un libro en el vector;
// FIN indica a un worker que no quedan mas libros para asignar.
class ColaTrabajo {
public:
  static constexpr int FIN = -1;
  ColaTrabajo() : mutex_(1), disponibles_(0) {}
  void encolar(int valor);
  int desencolar();
  bool vacia();
private:
  std::queue<int> cola_;
  Semaforo mutex_;       // seccion critica: estructura FIFO
  Semaforo disponibles_; // cuenta libros/avisos disponibles
};
#endif
