#ifndef COLORS_H
#define COLORS_H

#define COR_BRANCO 		"\e[7m"
#define COR_VERMELHO	"\e[101m"
#define COR_VERDE 		"\e[102m"
#define COR_AZUL 		"\e[104m"
#define COR_PRATA 		"\e[100m"
#define COR_PRETO 		"\e[40m"
#define COR_FIM			"\e[0m"

typedef enum{
	BRANCO,
	VERMELHO,
	VERDE,
	AZUL,
	PRATA,
	PRETO,
	NUM_CORES,
} CORES;

typedef enum{
	FIAT,
	FORD,
	CHEVROLET,
	TOYOTA,
	NISSAN,
	VOLKSWAGEN,
	NUM_MODELOS,
} MODELOS;

#endif //COLORS.H
