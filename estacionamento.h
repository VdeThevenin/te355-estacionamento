#ifndef ESTACIONAMENTO_H
#define ESTACIONAMENTO_H


#include "main.h"
#include "carro.h"


#define ESTACIONAMENTO_COLUNAS 			10
#define ESTACIONAMENTO_LINHAS			10
#define ESTACIONAMENTO_VAGAS 			ESTACIONAMENTO_COLUNAS*ESTACIONAMENTO_LINHAS

#define ESTACIONAMENTO_N_CANCELAS 		20

#define ESTACIONAMENTO_TRAFEGO_DIARIO	10000

typedef struct ESTATISTICA_TAG{
	uint16_t carros_passados;
	uint16_t tempo_aberto;
	float media_tempo_espera;
	float media_tempo_permanencia;
	float media_ocupacao;
} ESTATISTICA;

typedef struct VAGA_TAG{
	uint32_t id;
	CARRO* carro_estacionado;
	bool ocupada;
} VAGA;

typedef struct VAGAS_DISPONIVEIS_TAG{
	VAGA* vagas[ESTACIONAMENTO_VAGAS];
	uint32_t quantidade;
} VAGAS_DISPONIVEIS;

typedef struct ESTACIONAMENTO_TAG{
	// TODO: IMPLEMENTAR, MAS DAI PRECISA DE MAIS UM SEMAFORO E ESSE RECURSO COMPARTILHADO, TENSO
	//uint32_t fila_cancelas[ESTACIONAMENTO_N_CANCELAS]; 
	VAGA vagas[ESTACIONAMENTO_LINHAS][ESTACIONAMENTO_COLUNAS];
	VAGAS_DISPONIVEIS vagas_disp;
	ESTATISTICA* stats;
} ESTACIONAMENTO;


extern VAGA* ticket;
extern ESTATISTICA estatistica;

void update_media_carro(uint16_t espera, uint16_t permanencia);

void* estacionamento_thread(void* none);

#endif //ESTACIONAMENTO.H
