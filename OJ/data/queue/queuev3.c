#include<stdio.h>

#define Queens 8  //����������Ĵ�С��Ҳ���ǻʺ����Ŀ

int a[Queens+1];    //�˻ʺ�����Ļʺ����ڵ�����λ�ã���1��ʼ�������Լ�1

int main(){
    int i, k, flag, not_finish=1, count=0;
    //���ڴ����Ԫ���±꣬��ʾǰi-1��Ԫ���ѷ���Ҫ�����ڴ����i��Ԫ��
    i=1;
    a[1]=1;  //Ϊ����ĵ�һ��Ԫ�ظ���ֵ

    printf("The possible configuration of 8 queens are:\n");

    while(not_finish){  //not_finish=l:������δ����
        while(not_finish && i<=Queens){  //������δ�����һ�û������Queens��Ԫ��
            for(flag=1,k=1; flag && k<i; k++) //�ж��Ƿ��ж���ʺ���ͬһ��
                if(a[k]==a[i])
                    flag=0;

            for (k=1; flag&&k<i; k++)  //�ж��Ƿ��ж���ʺ���ͬһ�Խ���
                if( (a[i]==a[k]-(k-i)) || (a[i]==a[k]+(k-i)) )
                    flag=0;

            if(!flag){  //������ì�ܲ�����Ҫ����Ҫ�������õ�i��Ԫ��
                if(a[i]==a[i-1]){  //��a[i]��ֵ�Ѿ�����һȦ׷��a[i-1]��ֵ
                    i--;  //�˻�һ����������̽����ǰһ��Ԫ��

                    if(i>1 && a[i]==Queens)
                        a[i]=1;  //��a[i]ΪQueensʱ��a[i]��ֵ��1
                    else
                        if(i==1 && a[i]==Queens)
                            not_finish=0;  //����һλ��ֵ�ﵽQueensʱ����
                        else
                            a[i]++;  //��a[il��ֵȡ��һ��ֵ
                }else if(a[i] == Queens)
                    a[i]=1;
                else
                    a[i]++;  //��a[i]��ֵȡ��һ��ֵ
            }else if(++i<=Queens)
                if(a[i-1] == Queens )
                    a[i]=1;  //��ǰһ��Ԫ�ص�ֵΪQueens��a[i]=l
                else
                    a[i] = a[i-1]+1;  //����Ԫ�ص�ֵΪǰһ��Ԫ�ص���һ��ֵ
        }

        if(not_finish){
            ++count;
            printf((count-1)%3 ? "\t[%2d]:" : "\n[%2d]:", count);

            for(k=1; k<=Queens; k++) //������
                printf(" %d", a[k]);
            
            if(a[Queens-1]<Queens )
                a[Queens-1]++;  //�޸ĵ����ڶ�λ��ֵ
            else
                a[Queens-1]=1;

            i=Queens -1;    //��ʼѰ����һ�����������Ľ�
        }
    }
}