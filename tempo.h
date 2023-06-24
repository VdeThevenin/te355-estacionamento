#ifndef TEMPO_H
#define TEMPO_H

#include "main.h"
#include <time.h>

struct TEMPORIZADOR_TAG;

typedef uint64_t (*fun)(struct TEMPORIZADOR_TAG* tmp);

struct TEMPORIZADOR_TAG{
	uint64_t ti;
	uint64_t tf;
	uint64_t dt;
	fun start;
	fun stop;
};

typedef struct TEMPORIZADOR_TAG TEMPORIZADOR;

extern TEMPORIZADOR* _TEMPORIZADOR_(void);

uint64_t _start_(TEMPORIZADOR* tmp);
uint64_t _stop_(TEMPORIZADOR* tmp);

#endif //TEMPO.H
