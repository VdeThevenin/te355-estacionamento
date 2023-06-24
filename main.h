#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>



typedef struct intervalo_tag{
	uint8_t min;
	uint8_t max;
} INTERVALO;

typedef enum{
	BALANCEADO1,
	BALANCEADO2,
	TRANQUILO,
	OCIOSO,
	CRITICO,
	SAO_PAULO,
	NOVA_IORQUE,
	MODOS_NUM
} MODOS;

#endif //MAIN.H
