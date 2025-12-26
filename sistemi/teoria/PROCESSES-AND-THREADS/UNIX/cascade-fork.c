#include <unistd.h>
#include <stdlib.h>

#define NUM_FORKS 10

int main(int a, char ** b){

	int residual_forks = NUM_FORKS;

	another_fork:

	//decremento num_fork
	residual_forks--;
	//chiamo fork -> sto nel parent
	if(fork()>0){
		//pausa indeterminata
		pause();
	}
	else{
		//sono il processo figlio
		//residual fork del figlio è uguale a quello del padre
		if(residual_forks>0){	;
			//vado al codice identificato dalla label
		 	goto another_fork;	
		}
	}
	pause();
	//tutti i processi entrano in pausa



}
