#include<stdio.h>

int x;
double *y;

void main(void){
	y = &x;
	
	printf("x address is %p \n",&x);
	printf("y address is %p\n",y);
	printf("x+1 address is %p\n",&x+1);
	printf("y+1 address is %p\n",y+1);
	
 	printf("x address is %p - y is %p | x address+1 is %p - y+1 is %p\n",&x,y,&x+1,y+1);
}
