#include "semaforos.h"
#include "estacionamento.h"


sem_t sem_vagas;
sem_t sem_cancelas_entrada;
sem_t sem_cancelas_saida;
sem_t mutex_vaga;
sem_t mutex_ticket;


void inic_semaforo(void){
	sem_init(&sem_vagas, 0, ESTACIONAMENTO_VAGAS);
	sem_init(&sem_cancelas_entrada, 0, ESTACIONAMENTO_N_CANCELAS);
	sem_init(&sem_cancelas_saida, 0, ESTACIONAMENTO_N_CANCELAS);
	sem_init(&mutex_vaga, 0, 1);
	sem_init(&mutex_ticket, 0, 1);
}
