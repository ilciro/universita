#include <stdio.h>

//in memoria sono uno accanto all'altro
int control_variable;
int v[10];

int main(int argc, char * argv[]){

	int index, value;

#ifdef IN_STACK
	int control_variable;
	int v[10];
#endif
	//dipende quale variabile vado a prendere
	//se uso in stack (def) uso quello nello stack
	//altrimenti quella globale
	control_variable = 1;

	while (control_variable == 1){
		//anche qui la quantità di memoria è sconosciuta
		scanf("%d%d",&index,&value);
		//puntamento+offest
		//rischiamo buffer overflow
		v[index] = value;
		printf("array elem at position %d has been set to value %d\n",index,v[index]);


	}

	printf("exited cycle\n");

	return 0;
}

