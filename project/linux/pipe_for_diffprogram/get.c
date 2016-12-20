/******************get.c*****************/
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

int main()
{
	char s[128];
	int fd = open("/tmp/fifo.tst", O_RDONLY);
	int fd2 = open("./b.txt", O_CREAT|O_WRONLY);
	memset(s, 0, 128);
	while(read(fd, s, 128) > 0) {
	     printf("%s", s);
	     write(fd2, s, 128);
		 printf("fd2=%d\n",fd2);
	}
	close(fd2);
	close(fd); 
	return 0;
}
