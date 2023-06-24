#ifndef TEMPO_H
#define TEMPO_H

#include "main.h"
#include <time.h>

#define TEMPO_MAX 20000

struct TEMPORIZADOR_TAG;

typedef uint64_t (*fun)(struct TEMPORIZADOR_TAG* tmp);

struct TEMPORIZADOR_TAG{
	time_t ti;
	time_t tf;
	uint16_t dt;
	fun start;
	fun stop;
};

typedef struct TEMPORIZADOR_TAG TEMPORIZADOR;

extern TEMPORIZADOR* _TEMPORIZADOR_(void);

time_t _start_(TEMPORIZADOR* tmp);
time_t _stop_(TEMPORIZADOR* tmp);

void * contador_tempo_thread(void* none);

#endif //TEMPO.H
