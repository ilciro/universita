/*
 * If you use different compilers, you get different outcomes!
 */


#include <stdio.h>

int *buggy_return(void)
{
	int a_variable = 10;
	return &a_variable;
}

void using_stack(void)
{
	char a_string[] = "Hello World!";
	printf("%s\n", a_string);
}

int main(int argc, char *argv[])
{
	int *dummy = buggy_return();
	printf("%d\n", *dummy);
	using_stack();
	printf("%d\n", *dummy);
	return 0;
}
