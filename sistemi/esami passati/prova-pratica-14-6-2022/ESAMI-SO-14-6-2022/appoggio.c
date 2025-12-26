/*

void *funct(int i,char *str,char *file)
{

	//sistemo semaforo
	oper.sem_op=-1; //prendo token
	oper.sem_flg=0; //setatto cosi
	oper.sem_num=0; //settato cosi
	
	 
	
redo:
	int ret=semop(arraya,&oper,1);
	if(ret==-1 && errno==EINTR) exit(-1); //errore 	
	if(ret==-1) goto redo;//ripeto se operazione è bloccata -> token non disponibile
	
	
	if((sd=creat(file,0666))==-1) exit(-1);	
	printf("file creato :%s\n",file);
	
	//apro i file
	if((dd=open(file,O_RDWR|O_CREAT|O_TRUNC,0666))==-1) exit(-1);
	
	
	//scrivo su file			
	write(dd,buff,strlen(str));
	
	//sistemo semaforo
	oper.sem_op=1; //do token
	oper.sem_flg=0; //setatto cosi
	oper.sem_num=i; //settato cosi
	
	 
	
redo1:
	 ret=semop(arraya,&oper,1);
	if(ret==-1 && errno==EINTR) exit(-1); //errore 	
	if(ret==-1) goto redo1;//ripeto se operazione è bloccata -> token non disponibile
	
	
	//return NULL;
	
}



int main(int argc, char** argv){

	//provo a splittare le stringhe
	
	int contNF=0;
	int contNS=0;
	int partenza=0;
	int arrivo=0;

	int semid;

	
	pid_t pid;
	
	
	

	
	if(argc<2) printf("errore nei parametri\n");
	
	
	
	
		//splitto i parametri		
		
		for(int i=0;i<argc-1;i++)
		{
			//printf("argv['%d']:-->%s\n",i,argv[i]);

			if(strcmp(argv[i],"-f")==0)
			{
				
				partenza=i;
			}
		}

		for (int j=partenza;j<argc;j++)
		{
			if(strcmp(argv[j],"-s")==0)
			{
							
				arrivo=j;
			}
		}

		int diff=arrivo-partenza-1;

		for(int k=arrivo+1;k<argc;k++)
		{
			contNS++;
		}
		printf("numero file :%d -- numero stringhe :%d\n",diff,contNS);
		
		if(diff!=contNS)
		{
			printf("le dimensioni sono diverse !!\n");
			exit(-1);
		}
		
		
		//generazione processi concorrenti usando i semafori
		
		
		//creo il vettore semaforico di n elementi
		arraya=semget(IPC_PRIVATE,contNS,IPC_CREAT|0666);
		
		//finisco di inizializzare array
		for (int i=arrivo+1;i<diff;i++)
		{
			semctl(	arraya,1,SETVAL,i);
		}
		
			ho questa situazuone
			
			sem 1 1 1 1
		
		
		for(int i=0;i<diff;i++)
		{

			pid=fork();
			if(pid==-1) exit(-1);
			if(pid==0){
				printf("genero processo i :%d\n",i);
			}
		}
		

		
		
		//leggo da input e trovo corrispondenza
		while (1)
		{
		
			
			
			if(fgets(buff,sizeof(buff),stdin)!=NULL)
			{
			 printf("Il testo letto è: %s", buff);
			 strtok(buff,"\n");
			 for(int i=arrivo+1;i<argc;i++)
			 {
			 	//vdere se mettere fork qui
			 	
			 	if(strcmp(buff,argv[i])==0)
			 	{
			 		printf("trovato a posizione i:%d\n",i);
			 		
			 		signal(SIGINT,funct(i,buff,argv[i]));
		 		
				}
			}
			 
			}
		}
		
		
	

	return 0;
}
