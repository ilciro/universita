
//please compile with -nostartfiles

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

//presente all'interno di qualche libreria esterna
//anche questo è array di puntatori
//inizializzato dal kernel del sistema operativo
extern char** environ;

void _start(void){
	//array di puntatori-> stringa 
   	 char ** addr = environ;

         printf("environ head pointer is at address: %lu\n",(unsigned long)environ);
	 
	 while(*addr){
	//premndo il valore (stringa) contenuta in addr
      	      printf("%s\n",*(addr));
	//vado alla cella successiva del mio array di puntatori
 	      addr++;
	}
	exit(0);

}
