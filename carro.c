#include "main.h"
#include "carro.h"
#include "tempo.h"
#include "semaforos.h"
#include "estacionamento.h"


// sim, o carro sai da concessionaria destinado a ir ao estacionamento kkkk
CARRO* concessionaria(INTERVALO* perm){
	CARRO *carro = (CARRO*)malloc(sizeof(CARRO));
	uint8_t cor = rand() % NUM_CORES;
	uint8_t modelo = rand() % NUM_MODELOS;
	carro->tempo_permanencia = (uint16_t)(rand() % perm->max + perm->min);
	carro->vaga = 0;
	
	switch(cor){
		case VERMELHO:
			carro->cor = COR_VERMELHO;
			break;
		case VERDE:
			carro->cor = COR_VERDE;
			break;
		case AZUL:
			carro->cor = COR_AZUL;
			break;
		case PRATA:
			carro->cor = COR_PRATA;
			break;
		case PRETO:
			carro->cor = COR_PRETO;
			break;
		case BRANCO:
		default:
			carro->cor = COR_BRANCO;
			break;
	}	
	
	switch(modelo){
		case FIAT:
			carro->modelo = 'F';
			break;
		case FORD:
			carro->modelo = 'D';
			break;
		case CHEVROLET:
			carro->modelo = 'C';
			break;
		case TOYOTA:
			carro->modelo = 'T';
			break;
		case NISSAN:
			carro->modelo = 'N';
			break;
		case VOLKSWAGEN:
		default:
			carro->modelo = 'V';
			break;
	}	
	
	return carro;
}

void* carro_thread(void* carro_chegando){
	
	CARRO* carro = (CARRO*)carro_chegando;
	
	// inicia o cronometro para contagem do tempo de espera
	carro->tempo_espera = _TEMPORIZADOR_();
	carro->tempo_espera->start(carro->tempo_espera);
	
	// o carro verifica se tem vaga no estacionamento
	sem_wait(&sem_vagas);
	
	//tem vaga, bora pra cancela
	sem_wait(&sem_cancelas_entrada);
	
	//cria uma fila de cancelas verificando vagas
	sem_wait(&mutex_vaga);
	
	
	// faz poll da disponibilidade do vaga
	do{
		// agenda um ticket
		sem_wait(&mutex_ticket);
		
		//o carro pega o ticket
		if (ticket != (void*)0){
			carro->vaga = ticket;
			carro->vaga->carro_estacionado = carro;
			carro->vaga->ocupada = true;
			carro->tempo_espera->stop(carro->tempo_espera);
			ticket = (void*)0;
		}
		
		//libera a fila do ticket		
		sem_post(&mutex_ticket);
	}
	while(carro->vaga == (void*)0);
	
	//libera pra proxima cancela conseguir um ticket
	sem_post(&mutex_vaga);
	
	// vaga encontrada, pode liberar a cancela
	sem_post(&sem_cancelas_entrada);
	
	// agora que o carro estacionou, ele vai ficar pelo tempo de permanencia dele	
	sleep(carro->tempo_permanencia);
	//HORA DE IR EMBORA, PANGUÁ!
	
	//passa na cancela de saida
	sem_wait(&sem_cancelas_saida);
	
	//ticket passado na cancela, vaga livre no sistema
	carro->vaga->carro_estacionado = (CARRO*)0;
	carro->vaga->ocupada = false;
	
	// libera a cancela
	sem_post(&sem_cancelas_saida);
	
	// libera a vaga no painel do estacionamento
	sem_post(&sem_vagas);

	// o carro toma seu rumo
	pthread_exit(NULL);
}
