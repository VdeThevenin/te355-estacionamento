#include "main.h"
#include "carro.h"
#include "estacionamento.h"
#include "semaforos.h"
#include "tempo.h"



const INTERVALO chegada[]     = {{0, 2}, {0, 1}, {0, 2}, {0, 4}, {0, 1}, {0, 1}, {0, 0}};
const INTERVALO permanencia[] = {{0, 2}, {0, 1}, {0, 1}, {0, 1}, {0, 5}, {5, 75}, {2, 5}};

MODOS verifica_modo(char *argv[]){
	MODOS modo;
	char * m = argv[1];
	
	if (!strcmp(m, "BALANCEADO1") || !strcmp(m, "balanceado1")){
		modo = BALANCEADO1;
	}
	else if (!strcmp(m, "BALANCEADO1") || !strcmp(m, "balanceado2")){
		modo = BALANCEADO2;
	}
	else if (!strcmp(m, "TRANQUILO") || !strcmp(m, "tranquilo")){
		modo = TRANQUILO;
	}
	else if (!strcmp(m, "OCIOSO") || !strcmp(m, "ocioso")){
		modo = OCIOSO;
	}
	else if (!strcmp(m, "CRITICO") || !strcmp(m, "critico")){
		modo = CRITICO;
	}
	else if (!strcmp(m, "SP") || !strcmp(m, "sp")){
		modo = SAO_PAULO;
	}
	else if (!strcmp(m, "NY") || !strcmp(m, "ny")){
		modo = NOVA_IORQUE;
	}
	return modo;
}

int main(int argc, char *argv[]) {
	MODOS modo = BALANCEADO1;
	
	if (argc == 2){
		modo = verifica_modo(argv);
	}

	
	srand((unsigned)time(NULL));
	
	// indice de identificação da thread, ou seja, do carro
	uint16_t idx;
	
	// status de retorno na criação e join de threads
	int status;
	
	// variaveis que recebem o tempo randomico de permanencia e de chegada
	uint8_t dwell_time, arrive_time;
	
	// array de threads relacionadas aos carros
	pthread_t car_thread_arr[ESTACIONAMENTO_TRAFEGO_DIARIO];
	
	// thread de grafico do estacionamento
	pthread_t graph_thread;
	
	// thread de grafico do estacionamento
	pthread_t tempo_thread;
	
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
	
	status = pthread_create (&tempo_thread, NULL, contador_tempo_thread, (void *) 0);
	
	CARRO** carros = (CARRO**)malloc(sizeof(CARRO*)*ESTACIONAMENTO_TRAFEGO_DIARIO);	
	
	// 0~2 	0~2 	Balanceado
	// 0~1 	0~1 	Balanceado
	// 0~2 	0~1 	Tranquilo
	// 0~4 	0~1 	Ocioso
	// 0~1	0~5 	Crítico
		
	for(idx=0; idx < ESTACIONAMENTO_TRAFEGO_DIARIO; idx++){
		carros[idx] = concessionaria(&permanencia[modo]);
	}
	
	// loop de criação das threads (carros)
	for (idx = 0; idx < ESTACIONAMENTO_TRAFEGO_DIARIO; idx++){
		
		// criação da thread que representa o carr_arr[id]
		status = pthread_create(&car_thread_arr[idx], &attr, carro_thread, (void *) carros[idx]);
	
		// verificação se ocorreu tudo certo na criação da thread
		if (status)
		{
			perror ("LOG: error creating car thread\n") ;
			exit (1) ;
		}
	
		// para criar uma nova thread, deve-se esperar o tempo dado entre
		// 0 e u8_ArriveTimeMax[mode], representando o intervalo entre a
		// chegada de um carro e outro
		sleep(rand()%(chegada[modo].max+1)+chegada[modo].min);
	}
	
	status = pthread_join(graph_thread, NULL);
	if (status)
	{
		perror ("LOG: pthread_join error") ;
		exit (1) ;
	}	
		
	// loop do join da thread, pq para continuar a função main
	// é necessário que todas as threads tenham terminadas
	// isso pq a media de tempo de espera só pode ser contabilizada apos 
	// todos os carros terem chegados
	for (idx=0; idx<ESTACIONAMENTO_TRAFEGO_DIARIO; idx++)
	{
		status = pthread_join(car_thread_arr[idx], NULL);
		if (status)
		{
			perror ("LOG: pthread_join error") ;
			exit (1) ;
		}	
	}
		
	// destruição do atributos e das threads
	pthread_attr_destroy(&attr);
	//pthread_exit(NULL);

	// fim, retorna 0 (OK)
	return 0;
}
