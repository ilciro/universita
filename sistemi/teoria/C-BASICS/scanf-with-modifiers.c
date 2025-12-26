#include<stdio.h>
#include<stdlib.h>


int main(int argc, char**argv){

	char *p = NULL;
	//alloca area di memoria di m byte
	scanf("%ms",&p);
	printf("%s\n",p);
	//devo liberare area di memoria
	free(p);
	p = NULL;

}
