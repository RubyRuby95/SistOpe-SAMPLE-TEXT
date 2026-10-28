#include "Semaforos.h"
#include <cerrno>   // errno, EINTR

// Crea el semaforo con su valor inicial (0 = no compartido entre procesos, solo entre threads)
Semaforo::Semaforo(unsigned int valorInicial){ sem_init(&sem, 0, valorInicial); }

// Libera los recursos del semaforo
Semaforo::~Semaforo(){ sem_destroy(&sem); }

void Semaforo::wait(){
  // sem_wait devuelve 0 si tuvo exito. Si una senal lo interrumpe (EINTR) se vuelve a intentar.
  while (sem_wait(&sem) != 0 && errno == EINTR) {}
}

// Libera el semaforo (suma 1 y despierta a quien espera)
void Semaforo::post(){ sem_post(&sem); }
