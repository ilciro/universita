#include <windows.h>
#include <stdio.h>

#define BUFSIZE 1024

int main(int argc,char **argv)
{
	HANDLE sd,dd;
	DWORD size,result;
	char buffer[BUFSIZE];
	
	if(argc!=3) /* controllo numero argomenti */
	{
		printf("sage: copia source target\n");
		return -1;
	}
	/*Apertura file sola lettura */
	sd=CreateFile(argv[1],GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(sd==INVALID_HANDLE_VALUE)
	{
		printf("cannot open file \n");
		return -1;
	}
	/*creazione del file di destinazione */
	dd=CreateFile(argv[2],GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBURE_NORMAL,NULL);
	if(dd==INVALID_HANDLE_VALUE)
	{
		printf("cannot open destination file \n");
		return -1;	
	}
	do // qui iniziamo operazioni copia
	{
		if(ReadFile(sd,buffer,BUFSIZE,&size,NULL)==0)
		{
			printf("cannot read from source file\n");
			return -1;
		}
		if(WriteFile(dd,buffer,size,&result,NULL)==0)
		{
			printf("cannot write to destination \n");
			return -1;
		}
	}while(size>0);
	CloseHandle(sd);
	CloseHandle(dd);
}
