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
	
	if (!strcmp(m, "BALANCEADO1")){
		modo = BALANCEADO1;
	}
	else if (!strcmp(m, "BALANCEADO2")){
		modo = BALANCEADO2;
	}
	else if (!strcmp(m, "TRANQUILO")){
		modo = TRANQUILO;
	}
	else if (!strcmp(m, "OCIOSO")){
		modo = OCIOSO;
	}
	else if (!strcmp(m, "CRITICO")){
		modo = CRITICO;
	}
	else if (!strcmp(m, "SP")){
		modo = SAO_PAULO;
	}
	else if (!strcmp(m, "NY")){
		modo = NOVA_IORQUE;
	}
	return modo;
}

int main(int argc, char *argv[]) {
	MODOS modo = CRITICO;
	uint16_t idx;
	int status;
	
	srand((unsigned)time(NULL));
	
	if (argc == 2){
		modo = verifica_modo(argv);
	}
	
	pthread_t threads_carros[ESTACIONAMENTO_TRAFEGO_DIARIO];
	pthread_t thread_estacionamento;
	pthread_t tempo_thread;
	pthread_attr_t attr;
	
	inic_semaforo(&attr);
	
	status = pthread_create (&thread_estacionamento, &attr, estacionamento_thread, (void *) 0);
	if (status){
		perror ("ERRO: deu ruim na criacao da thread estacionamento\n");
		exit(1);
	}
	
	status = pthread_create (&tempo_thread, NULL, contador_tempo_thread, (void *) 0);
	
	CARRO** carros = (CARRO**)malloc(sizeof(CARRO*)*ESTACIONAMENTO_TRAFEGO_DIARIO);	
		
	for(idx=0; idx < ESTACIONAMENTO_TRAFEGO_DIARIO; idx++){
		carros[idx] = concessionaria(&permanencia[modo]);
	}
	
	for (idx = 0; idx < ESTACIONAMENTO_TRAFEGO_DIARIO; idx++){
		status = pthread_create(&threads_carros[idx], &attr, carro_thread, (void *)carros[idx]);
		if (status){
			perror("ERRO: deu ruim na criacao das threads dos carros\n");
			exit(1);
		}
	
		sleep(rand()%(chegada[modo].max+1)+chegada[modo].min);
	}
	
	status = pthread_join(thread_estacionamento, NULL);
	if (status){
		perror ("ERRO: deu xabu no join da thread do estacionamento\n");
		exit(2);
	}	
		
	for (idx=0; idx<ESTACIONAMENTO_TRAFEGO_DIARIO; idx++){
		status = pthread_join(threads_carros[idx], NULL);
		if (status){
			perror ("ERRO: deu xabu no join das threads dos carros\n");
			exit(2);
		}	
	}
	
	pthread_attr_destroy(&attr);

	return 0;
}
