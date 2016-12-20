#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

int open_from_file(char *pathname)
{
	int from_id=open(pathname,O_RDONLY);
	if(from_id>=0)
		return from_id;
	printf("opening %s faild\n",pathname);
	exit(EXIT_FAILURE);
	return -1;
}

int open_to_file(char *pathname)
{
	int to_id=open(pathname,O_RDWR|O_CREAT,777);
	if(to_id>=0)
		return to_id;
	printf("opening %s faild\n",pathname);
	exit(EXIT_FAILURE);
	return -1;
}

int copy_to_file(int from_id,int to_id)
{
	int byte_read,byte_write;
	char buffer[BUFFER_SIZE];
	char *ptr;
	
	while(byte_read=read(from_id,buffer,BUFFER_SIZE))
	{
		if((-1==byte_read)&&(errno!=EINTR))
			break;
		else if(byte_read>0)
		{
			ptr=buffer;
			while(byte_write=write(to_id,ptr,byte_read))
			{
				if((-1==byte_write)&&(errno!=EINTR))
					break;
				else if(byte_read==byte_write)
					break;
				else if(byte_write>0)
				{
					ptr+=byte_write;
					byte_read-=byte_write;
				}
			}
			if(byte_write==-1)
				break;
			
		}
		close(from_id);
		close(to_id);
		exit(EXIT_SUCCESS);
	}
}

int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		printf("inout format must be error an example can be like this:./cp pathname1 pathname2");
	}
	else
	{
		copy_to_file(open_from_file(argv[1],open_to_file(argv[2])));
		printf("copy finished\n");
	}
	return 0;
}
