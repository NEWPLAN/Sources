/******************send.c*****************/
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main()
{
	char s[128];
	int fd;
	FILE *fp;
	fp = fopen("./a.txt", "r");
	mkfifo("/tmp/fifo.tst", 0644);
	fd = open("/tmp/fifo.tst", O_WRONLY);
	while(fgets(s, 127, fp) != NULL) {
		write(fd, s, strlen(s));
		printf("%s",s);
	}
	close(fd);
	fclose(fp);
	unlink("/tmp/fifo.tst");

	return 0;
}


