#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>

using namespace std;
const string ans[10]={"zero","one","two","three","four","five","six","seven","eight","nine"};
int main(void)
{
	int sums=0;
	char pp[20]={0};
	char nss[105];
	scanf("%s",nss);
	
	int aa=strlen(nss);
	for(int a=0;a<aa;a++)
		sums+=nss[a]-'0';
	
	sprintf(pp,"%d",sums); 
	int bbb=strlen(pp);
	bool flags=false;
	for(int t=0;t<bbb-1;t++)
		cout<<ans[pp[t]-'0']<<" "; 
	cout<<ans[pp[bbb-1]-'0']; 
	return 0;
}