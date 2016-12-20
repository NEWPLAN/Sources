#include <stdio.h>
int main(int argc,char*argv[])
{
	if(argc<2)
	{
		printf("error in inout file \n");
		return 0;
	}
	FILE* in =fopen(argv[1],"rb");
	FILE* out=fopen(argv[2],"wb");
	if(!in||!out)
	{
		printf("error in file open or write \n");
		return 0;
	}
	while(!feof(in))
	{
		char pp;
		fread(&pp,1,1,in);
		fwrite(&pp,1,1,out);
	}
	printf("file copy finished\n");
	fclose(in);
	fclose(out);
	return 0;
}

