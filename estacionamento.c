#include "estacionamento.h"
#include "semaforos.h"
#include "cores.h"

VAGA* ticket = (void*) 0;
ESTATISTICA estatistica = {0, 0, 0.0f, 0.0f, 0.0f};

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

void update_media_carro(uint16_t espera, uint16_t permanencia){
	sem_wait(&mutex_media);
		estatistica.media_tempo_espera *= estatistica.carros_passados;
		estatistica.media_tempo_espera += (float)espera;
		estatistica.media_tempo_espera /= (float)(estatistica.carros_passados+1);
		
		estatistica.media_tempo_permanencia *= estatistica.carros_passados;
		estatistica.media_tempo_permanencia += (float)permanencia;
		estatistica.media_tempo_permanencia /= (float)++estatistica.carros_passados;
	sem_post(&mutex_media);
}

void update_media_estacionamento(ESTACIONAMENTO *e){
	sem_wait(&mutex_media);
		e->stats->media_ocupacao *= e->stats->tempo_aberto;
		e->stats->media_ocupacao += (float)(ESTACIONAMENTO_VAGAS - e->vagas_disp.quantidade);
		e->stats->media_ocupacao /= (float)(e->stats->tempo_aberto+1);
	sem_post(&mutex_media);
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

void printa_estatistica(ESTACIONAMENTO* e){
	sem_wait(&mutex_media);
	printf("\n");
	printf("*********************************\n");
	printf("* Tempo: %d            \t\t*\n", e->stats->tempo_aberto);
	printf("* Carros Totais: %d    \t\t*\n", e->stats->carros_passados);
	printf("* Medias:              \t\t*\n");
	printf("* \tEspera: %.2f       \t*\n", e->stats->media_tempo_espera);
	printf("* \tPermanencia: %.2f  \t*\n", e->stats->media_tempo_permanencia);
	printf("* \tOcupacao: %.2f     \t*\n", e->stats->media_ocupacao);
	printf("*********************************\n");
	sem_post(&mutex_media);
}

void mostra_estacionamento(ESTACIONAMENTO* e){
	
	uint8_t i, j;
	
	printa_vagas(e);
				
	for (i=0; i<ESTACIONAMENTO_LINHAS; i++){
		printf("\t");
		for (j=0; j < ESTACIONAMENTO_COLUNAS; j++){
			printf("##########");
		}
		printf("##\n\t");
		
		for (j=0; j < ESTACIONAMENTO_COLUNAS; j++){
			if (!e->vagas[i][j].ocupada){
				printf(" |        ");
			}
			else{
				printf(" | ");
				printf("%s", e->vagas[i][j].carro_estacionado->cor);
				printf("%s", e->vagas[i][j].carro_estacionado->placa);
				printf(COR_FIM);
			}
		}
		printf(" |\n\n");
	}
	
	printa_estatistica(e);
	
}

void* estacionamento_thread(void* none){
	
	static ESTACIONAMENTO estacionamento;
	memset(estacionamento.vagas, 0x00, sizeof(VAGA)*100);
	estacionamento.stats = &estatistica;
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
		
		update_vagas_disponiveis(&estacionamento);
		
		update_media_estacionamento(&estacionamento);
		
		mostra_estacionamento(&estacionamento);
		
		sem_post(&mutex_ticket);
		

		
	}
	
	pthread_exit(NULL);
}
