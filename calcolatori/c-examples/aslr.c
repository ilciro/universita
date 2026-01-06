#include <stdio.h>

void function(void)
{
	printf("This is a functionn");
}

int main(void)
{
	printf("Function is at address %p\n", function);
	return 0;
}
