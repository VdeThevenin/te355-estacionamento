#ifndef SEMAFOROS_H
#define SEMAFOROS_H

#include "main.h"
#include <pthread.h>
#include <semaphore.h>

extern sem_t sem_vagas;
extern sem_t sem_cancelas_entrada;
extern sem_t sem_cancelas_saida;
extern sem_t mutex_vaga;
extern sem_t mutex_ticket;

extern void inic_semaforo(void);

#endif //SEMAFOROS_H