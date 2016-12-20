#include <iostream>

using namespace std;

#define  MYDATA long long
const MYDATA MOD=1000000007;


#define  AMS 100005
MYDATA pp[AMS];
MYDATA asist[2*AMS];
void myinit()
{
	for(int i=0;i<AMS;i++)
	{
		/*算出五角数（正常五角数为1,5,12,22.... k*(3*k-1)/2）*/
		/*此部分，需要算出的是分割函数(1,2,5,7,12,15,22,26.....
		[k*(3*k-1)/2,k*(3*k+1)/2 ])*/
		asist[2*i]=i*(i*3-1)/2;
		asist[2*i+1]=i*(i*3+1)/2;
	}
}
void mymethod()
{
	pp[1]=1;
	pp[2]=2;
	pp[0]=1;
	for(int i=3;i<AMS;i++)
	{
		int k=0;
		int flags;
		pp[i]=0;
		/*pp[n]=pp[n-1]+pp[n-2]-pp[n-5]-pp[n-7]+pp[12]+pp[15]
		-.... ..... [+pp[n-k*[3k-1]/2]  + pp[n-k*[3k+1]/2]]*/
		for(int j=2;asist[j]<=i;j++)
		{
			/*说明：式子中+MOD是必须的，否则输出可能出错（有可能为负数）*/
			flags=k&2;
			if(!flags)
				pp[i]=(pp[i]+pp[i-asist[j]]+MOD)%MOD;
			else
				pp[i]=(pp[i]-pp[i-asist[j]]+MOD)%MOD;
			k++;
			k%=8;
		}
	}

	
	
}


int main()
{ 
	int t,n;

	myinit();
	mymethod();
  
    cin>>t;
    while(t--)
    {
        cin>>n;
		cout<<pp[n]<<endl;
    }

    return 0;
}
