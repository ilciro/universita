/******************************************************************
Welcome to the Operating System examination

You are editing the '/home/esame/prog.c' file. You cannot remove 
this file, just edit it so as to produce your own program according to
the specification listed below.

In the '/home/esame/'directory you can find a Makefile that you can 
use to compile this program to generate an executable named 'prog' 
in the same directory. Typing 'make posix' you will compile for 
Posix, while typing 'make winapi' you will compile for WinAPI just 
depending on the specific technology you selected to implement the
given specification. Most of the required header files (for either 
Posix or WinAPI compilation) are already included in the head of the
prog.c file you are editing. 

At the end of the examination, the last saved snapshot of this file
will be automatically stored by the system and will be then considered
for the evaluation of your exam. Modifications made to prog.c which are
not saved by you via the editor will not appear in the stored version
of the prog.c file. 
In other words, unsaved changes will not be tracked, so please save 
this file when you think you have finished software development.
You can also modify the Makefile if requesed, since this file will also
be automatically stored together with your program and will be part
of the final data to be evaluated for your exam.

PLEASE BE CAREFUL THAT THE LAST SAVED VERSION OF THE prog.c FILE (and of
the Makfile) WILL BE AUTOMATICALLY STORED WHEN YOU CLOSE YOUR EXAMINATION 
VIA THE CLOSURE CODE YOU RECEIVED, OR WHEN THE TIME YOU HAVE BEEN GRANTED
TO DEVELOP YOUR PROGRAM EXPIRES. 


SPECIFICATION TO BE IMPLEMENTED:
Si sviluppi una applicazione che riceva tramite argv[] la seguente linea di comando

    nome_prog -f file1 [file2] ... [fileN] -s stringa1 [stringa2] ... [stringaN] 
    
indicante N nomi di file (con N > 0) ed N ulteriori stringhe (il numero dei nomi dei 
file specificati deve corrispondere al numero delle stringhe specificate).

L'applicazione dovra' generare N processi figli concorrenti, in cui l'i-esimo di questi 
processi effettuera' la gestione dell'i-esimo dei file identificati tramite argv[].


Tale file dovra' essere rigenerato allo startup dell'applicazione.


Il main thread del processo originale dovra' leggere indefinitamente stringhe da 
standard input e dovra' comparare ogni stringa letta che le N stringhe ricevute in 
input tramite argv[].


Nel caso in cui la stringa letta sia uguale alla i-esima delle N stringhe ricevuta 
in input, questa dovra' essere comunicata all'i-esimo processo figlio in modo che questo 
la possa inserire in una linea del file di cui sta effettuando la gestione.

 Invece, 
se il main thread legge una stringa non uguale ad alcuna delle N stringhe ricevute 
in input, questa stringa dovra essere comunicata a tutti gli N processi figli
attivi, che la dovranno scrivere sui relativi file in una nuova linea.

L'applicazione dovra' gestire il segnale SIGINT (o CTRL_C_EVENT nel caso
WinAPI) in modo tale che quando uno qualsiasi dei processi figli venga colpito 
dovra' riportare il contenuto del file da esso correntemente gestito in un file
con lo stesso nome ma con suffisso "_backup".  Invece il processo originale non dovra'
terminare o eseguire alcuna attivita' in caso di segnalazione.

In caso non vi sia immissione di dati sullo standard input e non vi siano segnalazioni, 
l'applicazione dovra' utilizzare non piu' del 5% della capacita' di lavoro della CPU.

*****************************************************************/
#ifdef Posix_compile
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <fcntl.h>
#else
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

//numero file
int f;
//numero stringhe
int s;
//array main -> mutex
int arrayMain;
//arrayProcessi
int arrayFiles;
//descrittore file
int sd,fd;
//retrurn delle syscall
int ret;
//struct per il semaforo
struct sembuf oper;
//buffer dove scrivere cose
char buff[1024];
//memory map
# define  PAGE_SIZE (4096)
# define NUM_TARGET_PAGES 10

//memoria condivisa per passare il valore
char *address_map;

//memoria condivisa per passare il valore
//char *address_map_trovato;

void run(int index);
void handle(int signal, siginfo_t * a, void *b);

//file
FILE *file;

//appoggio argv
char **argvA;

//struct sigset
sigset_t set;
//struct sigaction
struct sigaction act;

//diff
int diff;






int main(int argc, char** argv){

	argvA=argv;
	
	int trovato=0;
	


	int a=0,b=0,c=0;
	for (int i=0;i<argc;i++) 
	{
		// trovo -f a pos 1
		if(strcmp(argv[i],"-f")==0)a=i;
		//trovo -s a pos 7
		if(strcmp(argv[i],"-s")==0)b=i;
	}
	for(int i=b+1;i<argc;i++) c++;
	 diff=b-a-1;
	printf("files : %d -- stringhe :%d\n",diff,c);
	
	
	
	
	//creo altri file
	for(int i=a+1;i<b;i++)
	{
		if((sd=creat(argv[i],0666))==-1) exit(-1);	
		else printf("ho creato file :%s\n",argv[i]);
	}

	//creo semaforo main
	arrayMain=semget(IPC_PRIVATE,1,IPC_CREAT|0666);
	if(arrayMain==-1) 
	{
		printf("errore nella allocazione del main sem\n");
		return -1;
	}
	//inizializzo il semaforo del main ad 1
	ret=semctl(arrayMain,0,SETVAL,1);
	if(ret==-1) {
		printf("errore nel semctl del main sem\n");
		return -1;
	}
	//creo semaforo files
	arrayFiles=semget(IPC_PRIVATE,diff,IPC_CREAT|0666);
	if(arrayFiles==-1){
		printf("errore nella allocazione del file sem\n");
		return -1;
	}
	for(int i=0;i<diff;i++)
	{
	
		
		ret=semctl(arrayFiles,i,SETVAL,0);
		if(ret==-1) {
		printf("errore nel semctl del file sem\n");
		return -1;
	
		}
	}
	
	//creo memory map 
	
	address_map =mmap(NULL,PAGE_SIZE*NUM_TARGET_PAGES,PROT_READ|PROT_WRITE,MAP_ANONYMOUS|MAP_SHARED,0,0);
	if(address_map==NULL) 
	{
		printf("errore allocazione memory map\n");
		return -1;
	}
	/*
	address_map_trovato =mmap(NULL,PAGE_SIZE*NUM_TARGET_PAGES,PROT_READ|PROT_WRITE,MAP_ANONYMOUS|MAP_SHARED,0,0);
	if(address_map_trovato==NULL) 
	{
		printf("errore allocazione memory map\n");
		return -1;
	}
	/*
	
	
	
	
	
	
	//sigignore
	/*
	sigfillset(&set);//svuoto maschera bit
	act.sa_sigaction=NULL;
	act.sa_mask=set;
	act.sa_flags=SA_SIGINFO;
	act.sa_restorer=NULL;
	
	sigaction(SIGINT,&act,NULL);
	sigprocmask(SIG_BLOCK,&set,NULL);
	*/
	for(int i=0;i<diff;i++)
	{
		if(fork()==0) continue;
		
		else run(i);
	}
	
	
	
	while(1)
	{
		
		//prendo gettone dal main
		oper.sem_num=0; //nr semaforo
		oper.sem_flg=0;//flags
		oper.sem_op=-1;//decremento il valore
		
	redo1 :
		ret=semop(arrayMain,&oper,1);
			if(ret==-1)
			 	if(errno==EINTR)
					goto redo1;
		
					
		//faccio operazioni
		while(fgets(buff, 1024, stdin)!=NULL)
		{

			
			buff[(strcspn(buff,"\n"))] = 0;
			
			

						
			for(int i=b+1,j=a+1;i<argc+1,j<diff+2;i++,j++)
			{
				
					
				
				if(strcmp(buff,argv[i])==0)
				{
					
					printf("i nell if vale :%d-- argv :%s \n",i,argv[i]);										
					memcpy(address_map, buff, sizeof(buff));				
					trovato=1;
				
					 //do gettone al sem dei files
					
					oper.sem_op = 1;
					oper.sem_flg = 0;
					oper.sem_num = j-2;
							
				redo2:	
					ret=semop(arrayFiles,&oper,1);
					if(ret==-1)
					 	if(errno==EINTR)
							goto redo2;	
					
				}
				if(trovato==0){
				
					//1) prendo gettone scrivo su file e lo torno al prossimo (i+1)
					//2) scrivo e emtto in wait 
					//3) struct di semafori
					//4) modifico la struttura del semaforo del main (da mutex a array di sem)
				
					/*
						/trova elementi
						int get_num_elements(int semid) {
						    struct semid_ds buf;
						    // IPC_STAT retrieves the semid_ds structure for the set
						    if (semctl(semid, 0, IPC_STAT, &buf) == -1) {
							perror("semctl IPC_STAT");
							return -1;
						    }
						    // sem_nsems contains the number of semaphores in the set
						    return buf.sem_nsems;
						}
					*/
				
					
				
				
			
			}
			
		munmap(&address_map,sizeof(buff));
		
		}
		
	
		
	}				

	
	
return 0;
}

void run(int index)
{

	
	
	
	
	
		int file_descriptor = open(argvA[index+2],O_RDWR|O_TRUNC, 0666);
		file = fdopen(file_descriptor, "w+");

	

	while(true){
	

		oper.sem_op =-1;
		oper.sem_flg = 0;
		oper.sem_num = index;

	redo3:	
		ret=semop(arrayFiles,&oper,1);
	        if(ret==-1)
		 	if(errno==EINTR)
				goto redo3;
		
		
		fprintf(file,"%s\n",address_map);
		fflush(file);
		fclose(file);

	
			
		//rido gettone al main
		oper.sem_op =1;
		oper.sem_flg = 0;
		oper.sem_num = 0;
		
				
	redo4:	
		ret=semop(arrayMain,&oper,1);
	        if(ret==-1)
		 	if(errno==EINTR)
				goto redo4;	
				
			
	}

}

void handle(int signal, siginfo_t * a, void *b)
{
	printf(" CTRL + C Pressed [*] \n");
	printf(" - -  %s - - \n", "exit");
}


/*
//creo file appoggio 
	if((sd=creat("appoggio",0666))==-1) exit(-1);	
	else printf("ho creato file :%s\n","appoggio");


int file_descriptor = open("appoggio",O_RDWR, 0666);
					file = fdopen(file_descriptor, "w+");

nt file_descriptor = open("appoggio",O_RDWR, 0666);
		file = fdopen(file_descriptor, "w+");	
		
	 if (file == NULL) { 
	 	printf("Errore nell'apertura del file.\n");
        	return ;
    	}else printf("file nel main aperto con successo \n");
    	*/









