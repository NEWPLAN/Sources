#include <stdio.h>
#include <stdlib.h>
/*********************************************
*  P214 �㷨6.3  �������˳������㷨
* ����˵����tblΪ������lenΪ������ĳ���
* RΪ�ֿ������˳���objΪ����ؼ���
**********************************************/
#define MAX (int)(((unsigned)(~((int)0)))>>1)
#define MIN (-MAX-1)

// ���������
typedef struct{
    int maxval;
    int start;
}Tindex, *Table;

int block_search(Tindex tbl[], int R[], int len, int obj)
{
    int id, k = 0;
    // ����ִ�п�����
    while( k < len && obj > tbl[k].maxval){
        k++;
    }

    if( k == len ){
        // ���obj��ֵ������˳�������ֵ������򷵻�-1��ʾ����ʧ��
        return -1;
    }
    else{
        // �ڿ���˳�����
        id = tbl[k].start;
        while( (R[id] != obj) && ( id < tbl[k+1].start) ){
            id++;
        }
        if( R[id] == obj ){
            return id; // ���ҳɹ������ز��ҵ��������±�
        }
        else{
            return -1; // ����ʧ��
        }
    }
}

int find_max(int R[], int start, int end)
{
    int i, maxval = MIN;
    //printf("**start = %d end = %d**\n", start, end);
    for(i = start; i < end; ++i){
        //printf("**%d**\n", R[i]);
        if(R[i] > maxval) maxval = R[i];
    }
    return maxval;
}

// ������������R��������������numΪR�ĳ��ȣ�����k��ʾ�����鳤��
// ��������ֵΪ������
Table build_idxtab(int R[], int num, int k)
{
    int i, len;
    if(num % k){
        len = ((num + k)/k) + 1;
    }
    else{
        len =  (num / k) + 1;
    }

    Table Tabidx = (Table)malloc(sizeof(Tindex)*len);

    //printf("**k = %d len = %d**\n", k, len);

    for(i = 0; i < (len -1); i++){
        if( (i*k+k) <= num ){
            Tabidx[i].maxval = find_max(R, i*k, i*k+k);
        }
        else{
            Tabidx[i].maxval = find_max(R, i*k, num);
        }
        Tabidx[i].start = k*i;
    }
    Tabidx[len -1].maxval = 0;
    Tabidx[len -1].start = num;
    return Tabidx;
}


int main()
{
    int id, k, num, obj, len;
    int i, j;
    Table pidx;

    int keys[] = {22, 12, 13, 8, 9, 20, 33, 42, 44, 38, 24, 48, 60, 58, 74, 49, 86, 53};
    num = sizeof(keys)/sizeof(keys[0]);

    k = 6;
    if(num % k){
        len = (num + k)/k;
    }
    else{
        len =  num / k;
    }
    pidx = build_idxtab(keys, num, k);

    printf("���������£�\n");
    for(i = 0; i < len; i++){
        printf("%d  %d \n", pidx[i].maxval, pidx[i].start);
    }

    printf("����������key����ֵ���ҵĽ�����£�\n");
    while(1){
        printf("\n���������Ԫ�أ�\n");
        scanf("%d", &obj);
        getchar();

        id = block_search( pidx, keys, len ,obj);

        printf("����ֵ = %d��  ", obj);
        if(id == -1){
            printf("��ѯ���������ʧ�ܣ�\n");
        }
        else{
            printf("��ѯ���������Ԫ��������key�е��±�Ϊ %d\n", id);
        }
    }

    free(pidx);
    system("pause");
    return 0;
}
