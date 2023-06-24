#include "tempo.h"

uint64_t _start_(TEMPORIZADOR* tmp){
	
	tmp->ti = (uint64_t)(clock()/CLOCKS_PER_SEC);
	tmp->tf = 0;
	tmp->dt = 0;
	return tmp->ti;
}

uint64_t _stop_(TEMPORIZADOR* tmp){
	tmp->tf = (uint64_t)(clock()/CLOCKS_PER_SEC);
	tmp->dt = tmp->tf - tmp->ti;
	return tmp->dt;
}

TEMPORIZADOR* _TEMPORIZADOR_(void){
	static TEMPORIZADOR tmp;
	tmp.start = _start_;
	tmp.stop = _stop_;	
	return &tmp;
}
