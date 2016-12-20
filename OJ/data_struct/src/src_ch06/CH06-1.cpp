#include <stdio.h>
#include <stdlib.h>
/*********************************************
*  P214 算法6.3  索引表的顺序查找算法
* 参数说明：tbl为索引表，len为索引表的长度
* R为分块有序的顺序表，obj为待查关键字
**********************************************/
#define MAX (int)(((unsigned)(~((int)0)))>>1)
#define MIN (-MAX-1)

// 索引表项定义
typedef struct{
    int maxval;
    int start;
}Tindex, *Table;

int block_search(Tindex tbl[], int R[], int len, int obj)
{
    int id, k = 0;
    // 首先执行块间查找
    while( k < len && obj > tbl[k].maxval){
        k++;
    }

    if( k == len ){
        // 如果obj的值超出了顺序表的最大值，则程序返回-1表示查找失败
        return -1;
    }
    else{
        // 在块内顺序查找
        id = tbl[k].start;
        while( (R[id] != obj) && ( id < tbl[k+1].start) ){
            id++;
        }
        if( R[id] == obj ){
            return id; // 查找成功，返回查找到的数组下标
        }
        else{
            return -1; // 查找失败
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

// 根据输入序列R建立索引表，参数num为R的长度，参数k表示索引块长度
// 函数返回值为索引表
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

    printf("索引表如下：\n");
    for(i = 0; i < len; i++){
        printf("%d  %d \n", pidx[i].maxval, pidx[i].start);
    }

    printf("对整型数组key进行值查找的结果如下：\n");
    while(1){
        printf("\n请输入待查元素：\n");
        scanf("%d", &obj);
        getchar();

        id = block_search( pidx, keys, len ,obj);

        printf("待查值 = %d：  ", obj);
        if(id == -1){
            printf("查询结果：查找失败！\n");
        }
        else{
            printf("查询结果：待查元素在数组key中的下标为 %d\n", id);
        }
    }

    free(pidx);
    system("pause");
    return 0;
}
