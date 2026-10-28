// ============================================================================
// ITEM 1.7 - USO DE SEMAFOROS
// Envoltorio (clase) sobre los semaforos POSIX (sem_t).
// Lo usan: la cola de trabajo (1.4, companero), el indice global (1.10)
// y el contador de progreso (1.11).
// ============================================================================
#ifndef SEMAFOROS_H
#define SEMAFOROS_H

#include <semaphore.h>   // sem_t, sem_init, sem_wait, sem_post, sem_destroy

class Semaforo {
public:
  // valorInicial = 1 -> semaforo binario (exclusion mutua)
  // valorInicial = 0 -> semaforo contador (cuenta elementos disponibles)
  explicit Semaforo(unsigned int valorInicial);
  ~Semaforo();                                  // destruye el semaforo
  Semaforo(const Semaforo&) = delete;           // un semaforo no se puede copiar
  Semaforo& operator=(const Semaforo&) = delete;

  void wait();   // P(): si el valor es 0 el thread se duerme; si no, lo decrementa y sigue
  void post();   // V(): incrementa el valor y despierta a un thread que espere

private:
  sem_t sem;     // semaforo POSIX que se envuelve
};

// "Guardia": toma el semaforo al crearse y lo libera al destruirse.
// Asi el semaforo siempre se libera al salir del bloque { }, aunque haya un return anticipado.
class Guardia {
public:
  explicit Guardia(Semaforo& s) : s(s) { s.wait(); }   // entra a la seccion critica
  ~Guardia() { s.post(); }                             // sale de la seccion critica
  Guardia(const Guardia&) = delete;
  Guardia& operator=(const Guardia&) = delete;

private:
  Semaforo& s;   // semaforo que esta tomando
};

#endif
