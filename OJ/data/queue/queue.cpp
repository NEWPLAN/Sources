#include<cstdio>
#include<cmath>


using namespace std;
int x[100];
bool place(int k)//����ʺ�k������x[k]���Ƿ�����ͻ
{
    int i;
    for(i=1;i<k;i++)
        if(x[k]==x[i]||abs(k-i)==abs(x[k]-x[i]))
            return false;
        return true;
}

void queue(int n)
{
	int ccs=0;
    int i,k;
    for(i=1;i<=n;i++)
        x[i]=0;
    k=1;
    while(k>=1)
    {
        x[k]=x[k]+1;   //����һ�з��õ�k���ʺ�
        while(x[k]<=n&&!place(k))
            x[k]=x[k]+1;//������һ��
        if(x[k]<=n&&k==n)//�õ�һ�����
        {
			printf("��%d��\t",++ccs);
            for(i=1;i<=n;i++)
                printf("%d ",x[i]);
            printf("\n");
        //return;//��return��ֻ�������һ�ֽ⣬����return����Լ������ݣ����ȫ���Ŀ��ܵĽ�
        }
        else if(x[k]<=n&&k<n)
            k=k+1;//������һ���ʺ�
        else
        {
            x[k]=0;//����x[k],����
            k=k-1;
        }
    }
}

int main()
{
   int n;
   printf("����ʺ����n:\n");
   scanf("%d",&n);
   queue(n);
   return 0;
}