#include <stdio.h>
#include <string.h>

char *gets(char *s) {
  int ch;
  char *p = s;

  while ( (ch=getchar()) != '\n' && ch != EOF) {
    *s++ = (char) ch;
  }
  *s = '\0';
  return p;
}


int main(int argc, char **argv)
{
	char buffer[64];
	gets(buffer);
	printf("Hello %s\n", buffer);
	return 0;
}

