/*²âÊÔÍ¨¹ı19/20*/
#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>

using namespace std;

int main(void)
{
	int sums=0;
	char pp[20]={0};
	char nss[105];
	scanf("%s",nss);
	if(nss[0]=='-')
	{
		printf("zero");
		return 0;
	}
	int aa=strlen(nss);
	for(int a=0;a<aa;a++)
		sums+=nss[a]-'0';
	
	sprintf(pp,"%d",sums);
	int bbb=strlen(pp);
	bool flags=false;
	for(int t=0;t<bbb;t++)
	{
		if(flags)
			printf(" ");
		flags=true;
		
		switch(pp[t])
		{
			case '0':
			printf("zero");
			break;
			case '1':
			printf("one");
			break;
			case '2':
			printf("two");
			break;
			case '3':
			printf("three");
			break;
			case '4':
			printf("four");
			break;
			case '5':
			printf("five");
			break;
			case '6':
			printf("six");
			break;
			case '7':
			printf("seven");
			break;
			case '8':
			printf("eight");
			break;
			case '9':
			printf("nine");
			break;
		}
	}
	return 0;
}