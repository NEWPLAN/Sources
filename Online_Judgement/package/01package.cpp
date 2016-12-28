/*
  ��������
*/
#include <cstdio>
#include <cstring>
#define MAXVAL 1001

int Weight[MAXVAL];
int Values[MAXVAL];

int status[MAXVAL][MAXVAL];
int visited[MAXVAL][MAXVAL];

#define MAXV(a,b)\
    (a)>(b)?(a):(b)

void inits(void)
{
    memset(Values,0,sizeof(Values));
    memset(Weight,0,sizeof(Weight));
    memset(status,0,sizeof(status));
    memset(visited,0,sizeof(visited));
}

/*********************************************0-1**************************************************/
int changestatus(int i, int j)
{

    if(i<=0||j<=0)
        return 0;
    if(j>=Weight[i])/*�˴�����Ҫ���ܹ�װ���±����*/
    {
        if(!visited[i-1][j])
        {
            status[i-1][j]=changestatus(i-1,j);
            visited[i-1][j]=1;
        }

        if(!visited[i-1][j-Weight[i]])
        {
            status[i-1][j-Weight[i]]=changestatus(i-1,j-Weight[i]);
            visited[i-1][j-Weight[i]]=1;
        }

        status[i][j]=MAXV(status[i-1][j],status[i-1][j-Weight[i]]+Values[i]);
    }
    else/*�����Ҫ���붪�±�����Ʒ*/
        status[i][j]=status[i-1][j];
    printf("%d\t%d\t%d\n",i,j,status[i][j]);
    return status[i][j];
}

/*
0-1��������
״̬ת�Ʒ���
f[i][v]=max{f[i-1][v], f[i-1][v-w[i]]+v[i]};
f[i][v]��ʾǰi�����װ������Ϊv�İ�������ļ�ֵ
w[i]���i������;
v[i]���i�ļ�ֵ;
*/
int pack_0_1(int n,int sizes)
{
    return changestatus(n,sizes);
}

void pack_0_1_test(void)
{
	int nums;
    int contains;/*the contain of packages*/
    while(scanf("%d",&contains)!=EOF)
    {
        if(!contains)
            break;
        scanf("%d",&nums);/*number of packages*/
        inits();
        for(int i=1; i<=nums; i++)
            scanf("%d%d",&Weight[i],&Values[i]);
        printf("=========max value is %d============\n",pack_0_1(nums,contains));
        for(int i=0; i<=nums; i++)
        {
            for (int j = 0; j <= contains; ++j)
            {
                printf("%4d  ",status[i][j]);
            }
            printf("\n");
        }
    }
}
/*********************************************0-1**************************************************/

/*********************************************��ȫ**************************************************/
/*
��N����Ʒ��һ������ΪV�ı�����ÿ����Ʒ�������޼����á���i����Ʒ�ķ�����c[i]����ֵ��w[i]��
��⽫��Щ��Ʒװ�뱳����ʹ��Щ��Ʒ�ķ����ܺͲ����������������Ҽ�ֵ�ܺ����
*/

int main(int argc,char* argv[])
{
    pack_0_1_test();
    return 0;
}
