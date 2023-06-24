#include "main.h"
#include "carro.h"
#include "estacionamento.h"
#include "semaforos.h"
#include <unistd.h>

const uint8_t u8_ArriveTimeMax[] = {2, 1, 2, 4, 1, 0, 1};
const uint8_t u8_DwellTimeMax[] = {2, 1, 1, 1, 5, 0, 200};

int main(int argc, char *argv[]) {
	
	srand((unsigned)time(NULL));
	
	// indice de identificação da thread, ou seja, do carro
	uint16_t id;
	
	// status de retorno na criação e join de threads
	int status;
	
	// variaveis que recebem o tempo randomico de permanencia e de chegada
	uint8_t dwell_time, arrive_time;
	
	// array de threads relacionadas aos carros
	pthread_t car_thread_arr[ESTACIONAMENTO_TRAFEGO_DIARIO];
	
	// thread de grafico do estacionamento
	pthread_t graph_thread;
	
	// atributo das threads (joinable)
	pthread_attr_t attr;
	
	// inicialização dos semáforos utilizados
	// com os numeros definidos em "car_park.h"
	inic_semaforo();
	
	// inicialização e setup do atributo das threads
	pthread_attr_init (&attr) ;
   	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE) ;
	
	
	status = pthread_create (&graph_thread, &attr, estacionamento_thread, (void *) 0);
	if (status)
	{
		perror ("LOG: error creating graph thread\n") ;
		exit (1) ;
	}
		
	// loop de criação das threads (carros)
	for (id = 0; id < ESTACIONAMENTO_TRAFEGO_DIARIO; id++){
		
		INTERVALO intervalo;
		intervalo.max = 200;
		intervalo.min = 0;
		
		CARRO* carro;
		carro = concessionaria(&intervalo);
		
		// criação da thread que representa o carr_arr[id]
		status = pthread_create (&car_thread_arr[id], &attr, carro_thread, (void *) carro) ;
	
		// verificação se ocorreu tudo certo na criação da thread
		if (status)
		{
			perror ("LOG: error creating car thread\n") ;
			exit (1) ;
		}
	
		// para criar uma nova thread, deve-se esperar o tempo dado entre
		// 0 e u8_ArriveTimeMax[mode], representando o intervalo entre a
		// chegada de um carro e outro
		sleep(1);
	}
	
	// loop do join da thread, pq para continuar a função main
	// é necessário que todas as threads tenham terminadas
	// isso pq a media de tempo de espera só pode ser contabilizada apos 
	// todos os carros terem chegados
	for (id=0; id<ESTACIONAMENTO_TRAFEGO_DIARIO; id++)
	{
		status = pthread_join (car_thread_arr[id], NULL) ;
		if (status)
		{
			perror ("LOG: pthread_join error") ;
			exit (1) ;
		}	
	}
	
	// destruição do atributos e das threads
	pthread_attr_destroy (&attr) ;
	pthread_exit (NULL) ;
	
	// fim, retorna 0 (OK)
	return 0;
}
