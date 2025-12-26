#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int main (int argc, char *argv[]) {

  int fd;

  if (argc!= 2) {
    printf("Syntax: write_on_file <file_name>\n");
    exit(-1);
  }
  
  //permessi -> o in alternativa codifica ottale
  fd=open(argv[1], O_CREAT| O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR);

  if (fd == -1) {
  	//il messaggio viene mandato su stdout
    perror("Open error: ");
    exit(2);
  }

  printf("fd=%d\n",fd);
  //chiudo stdout
  close (1);
  dup(fd);
  execve("./writer", NULL, NULL);
  perror("Exec error: ");
  exit(3);
}

