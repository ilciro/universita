#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#define SLEEP_PERIOD 5

void generic_handler(int signal, siginfo_t * a, void *b){
  sigset_t set;
  printf("received signal is %d\n",signal);
  fflush(stdout);

}

int main(int argc, char **argv){

  int  i;
  //registro 0/1 per i segnali
  sigset_t set; //bit mask
  struct sigaction act;
  //la riempio di tutti i segnali
  sigfillset(&set);
  //gestore da impostare
  act.sa_sigaction = generic_handler; 
  //maschera dei segnali bloccati 
  //mentre il gestore esegue
  //per arrivo segnalazione
  act.sa_mask =  set;
  //gestore che parte
  act.sa_flags = SA_SIGINFO;
  //valore di default
  act.sa_restorer = NULL;
  
  sigaction(SIGINT,&act,NULL);
  
  //il gestore dei segnali è bloccato
  sigprocmask(SIG_BLOCK,&set,NULL);

  while(1) {
  	//dormo per 5 secondi
	sleep(SLEEP_PERIOD);
	//chiedo al kernel se ci
	//sono segnali pendenti
	sigpending(&set);
	//tra i segnali pendenti cìè sig*?
	if(sigismember(&set,SIGINT)){
	  //svuoto la maschera dei segnali
	  sigemptyset(&set);
	  //aggiungo segnale
	  sigaddset(&set,SIGINT);
	  //sblocco maschera dove solo SIGINT
	  sigprocmask(SIG_UNBLOCK,&set,NULL);
	  //riblocco maschera di segnalazione
	  sigprocmask(SIG_BLOCK,&set,NULL);
	}
  }

}
