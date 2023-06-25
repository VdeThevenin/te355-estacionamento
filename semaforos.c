#include "semaforos.h"
#include "estacionamento.h"


sem_t sem_vagas;
sem_t sem_cancelas_entrada;
sem_t sem_cancelas_saida;
sem_t mutex_vaga;
sem_t mutex_ticket;
sem_t mutex_media;

void inic_semaforo(pthread_attr_t* attr){
	sem_init(&sem_vagas, 0, ESTACIONAMENTO_VAGAS);
	sem_init(&sem_cancelas_entrada, 0, ESTACIONAMENTO_N_CANCELAS);
	sem_init(&sem_cancelas_saida, 0, ESTACIONAMENTO_N_CANCELAS);
	sem_init(&mutex_vaga, 0, 1);
	sem_init(&mutex_ticket, 0, 1);
	sem_init(&mutex_media, 0, 1);
	
	pthread_attr_init(attr) ;
   	pthread_attr_setdetachstate(attr, PTHREAD_CREATE_JOINABLE) ;
}
