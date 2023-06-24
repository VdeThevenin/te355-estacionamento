#ifndef CARRO_H
#define CARRO_H

#include "cores.h"
#include "tempo.h"

typedef struct VAGA_TAG VAGA;

typedef struct CARRO_TAG{
	char* cor;
	char* modelo;
	char* placa;
	VAGA* vaga;
	uint16_t tempo_permanencia;
	struct TEMPORIZADOR_TAG* tempo_espera;
} CARRO;

void* carro_thread(void* carro_chegando);

CARRO* concessionaria(INTERVALO* perm);

#endif //CARRO.H
