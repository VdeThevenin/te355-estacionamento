#include "tempo.h"
#include "main.h"
#include "semaforos.h"
#include "estacionamento.h"

time_t _start_(TEMPORIZADOR* tmp){
	tmp->ti = time(NULL);
	tmp->tf = 0;
	tmp->dt = 0;
	return tmp->ti;
}

time_t _stop_(TEMPORIZADOR* tmp){
	tmp->tf = time(NULL);
	tmp->dt = (uint16_t)(tmp->tf - tmp->ti);
	return tmp->tf;
}

TEMPORIZADOR* _TEMPORIZADOR_(void){
	static TEMPORIZADOR tmp;
	tmp.start = _start_;
	tmp.stop = _stop_;	
	return &tmp;
}

void * contador_tempo_thread(void* none){
	uint16_t t = 0;
	while(t < TEMPO_MAX){
		sem_wait(&mutex_media);
		if (&estatistica != 0){
			t = estatistica.tempo_aberto++;
		}
		sem_post(&mutex_media);
		sleep(1);
	}
	
	pthread_exit(NULL);
}
