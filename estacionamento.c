#include "estacionamento.h"
#include "semaforos.h"
#include "cores.h"

VAGA* ticket = (void*) 0;

void update_vagas_disponiveis(ESTACIONAMENTO *e){
	uint8_t i, j;
	
	e->vagas_disp.quantidade = 0;
	
	for (i=0; i<ESTACIONAMENTO_LINHAS; i++){
		for (j=0; j < ESTACIONAMENTO_COLUNAS; j++){
			if (!e->vagas[i][j].ocupada){
				e->vagas_disp.vagas[e->vagas_disp.quantidade++] = &(e->vagas[i][j]);
			}
		}
	}
}

VAGA* retorna_vaga_disponivel_aleatoria(ESTACIONAMENTO *e){
	uint8_t idx = 0;
	
	update_vagas_disponiveis(e);
	
	if (e->vagas_disp.quantidade == 0){
		return (void*)0;
	}
	
	idx = rand() % e->vagas_disp.quantidade;
	return e->vagas_disp.vagas[idx];
}

void printa_vagas(ESTACIONAMENTO* e){
	
	system("cls");
	
	printf("");
	printf("\t*********************\n");
	printf("\t* VAGAS DISPONIVEIS *\n");
	if (e->vagas_disp.quantidade < 100){
		printf("\t**        %d       **\n", e->vagas_disp.quantidade);
	}
	else{
		printf("\t**       %d       **\n", e->vagas_disp.quantidade);
	}
	printf("\t*********************\n");
}

void mostra_estacionamento(ESTACIONAMENTO* e){
	
	uint8_t i, j;
	
	printa_vagas(e);
				
	for (i=0; i<ESTACIONAMENTO_LINHAS; i++){
		printf("\t");
		for (j=0; j < ESTACIONAMENTO_COLUNAS; j++){
			printf("####");
		}
		printf("##\n\t");
		
		for (j=0; j < ESTACIONAMENTO_COLUNAS; j++){
			if (!e->vagas[i][j].ocupada){
				printf(" |  ");
			}
			else{
				printf(" | ");
				printf("%s", e->vagas[i][j].carro_estacionado->cor);
				printf("%c", e->vagas[i][j].carro_estacionado->modelo);
				printf(COR_FIM);
			}
		}
		printf(" |\n\n");
	}
	
}

void* estacionamento_thread(void* none){
	
	ESTACIONAMENTO estacionamento;
	memset(estacionamento.vagas, 0x00, sizeof(VAGA)*100);
	update_vagas_disponiveis(&estacionamento);
	
	int16_t trafego_ate_agora = -1;
	
	while(trafego_ate_agora < ESTACIONAMENTO_TRAFEGO_DIARIO){
	
		// verifica se nao ha nenhum carro registrando um ticket
		sem_wait(&mutex_ticket);
		// nao há
	
		if (ticket == (void*)0){
			ticket = retorna_vaga_disponivel_aleatoria(&estacionamento);
			trafego_ate_agora++;
		}
		
		mostra_estacionamento(&estacionamento);
		
		sem_post(&mutex_ticket);
		
		// para nao flickar tanto
		// pode parecer que influencia na espera dos carros,
		// mas o comportamento permanece o mesmo, mesmo sem o sleep
		sleep(1);
	}
	
	pthread_exit(NULL);
}
