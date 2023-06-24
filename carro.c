#include "main.h"
#include "carro.h"
#include "tempo.h"
#include "semaforos.h"
#include "estacionamento.h"

char* SENATRAN(){
	char* placa = (char*)malloc((7 + 1) * sizeof(char));
    const char* letras = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char* numeros = "0123456789";

    placa[0] = letras[rand() % 26];
    placa[1] = letras[rand() % 26];
    placa[2] = letras[rand() % 26];
    placa[3] = numeros[rand() % 10];
    placa[4] = letras[rand() % 10];
    placa[5] = numeros[rand() % 10];
    placa[6] = numeros[rand() % 10];
    placa[7] = '\0';

    return placa;
}

// sim, o carro sai da concessionaria destinado a ir ao estacionamento kkkk
CARRO* concessionaria(INTERVALO* perm){
	CARRO *carro = (CARRO*)malloc(sizeof(CARRO));
	uint8_t cor = rand() % NUM_CORES;
	uint8_t modelo = rand() % NUM_MODELOS;
	carro->tempo_permanencia = (uint16_t)(rand() % (perm->max+1) + perm->min);
	carro->vaga = 0;
	carro->placa = SENATRAN();
	
	
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
			carro->modelo = "Fiat";
			break;
		case FORD:
			carro->modelo = "Ford";
			break;
		case CHEVROLET:
			carro->modelo = "Chevrolet";
			break;
		case TOYOTA:
			carro->modelo = "Toyota";
			break;
		case NISSAN:
			carro->modelo = "Nissan";
			break;
		case VOLKSWAGEN:
		default:
			carro->modelo = "Volkswagen";
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
			update_media_carro(carro->tempo_espera->dt, carro->tempo_permanencia);
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
		
	sem_wait(&mutex_ticket);
	//ticket passado na cancela, vaga livre no sistema
	carro->vaga->carro_estacionado = (void*)0;
	carro->vaga->ocupada = false;
	
	sem_post(&mutex_ticket);
	
	// libera a cancela
	sem_post(&sem_cancelas_saida);
	
	// libera a vaga no painel do estacionamento
	sem_post(&sem_vagas);

	// o carro toma seu rumo
	pthread_exit(NULL);
}
