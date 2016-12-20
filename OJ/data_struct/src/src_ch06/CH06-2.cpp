#include <stdio.h>
#include <stdlib.h>
/*********************************************
*  P220 算法6.5  开放地址法
* 线性探测再散列的散列插入、删除与查找算法
* 参数说明：HashTable为哈希表，key为待查关键字
**********************************************/
#define LEN_HT 16  // 哈希表的表长
#define PVAL 13      // 除留余数法使用的除数

// 哈希函数
int hash(int key)
{
    return(key % PVAL);
}

// 线性探测再散列：插入操作
int linear_probing_ins(int HashTable[],int key)
{  int idx, d = 0;
   idx = hash(key);

   while((d < LEN_HT) &&( HashTable[(idx+d)%LEN_HT] != key) && (HashTable[(idx+d)%LEN_HT] != -1)){
      d++;
   }
   //printf("%d:%d\n", key, d);
   if( d == LEN_HT ){
       return -1;   //表满,退出
   }

   idx = (idx + d) % LEN_HT;
   if( HashTable[idx] <= 0 ){
       // 位置idx尚未使用，执行插入操作
       HashTable[idx] = key;
       return idx;
   }
   else if( HashTable[idx] == key){
       // 表中已有该元素
       return idx;
   }
}

// 线性探测再散列：查找操作
int linear_probing_search(int HashTable[],int key)
{  int idx, d = 0;
   idx = hash(key);
   //printf("%d\n", idx);
   while((d < LEN_HT) &&( HashTable[(idx+d)%LEN_HT] != key) && (HashTable[(idx+d)%LEN_HT] != -1)){
      d++;
   }
   idx = (idx + d) % LEN_HT;
   if( HashTable[idx] == key ){
       return idx;
   }
   else{
       return -1;
   }
}

// 线性探测再散列：删除操作
int linear_probing_del(int HashTable[],int key)
{  int idx, d = 0;
   idx = hash(key);

   while((d < LEN_HT) &&( HashTable[(idx+d)%LEN_HT] != key) && (HashTable[(idx+d)%LEN_HT] != -1)){
      d++;
   }
   idx = (idx + d) % LEN_HT;
   printf("\n idx = %d\n", idx);
   if( HashTable[idx] == key ){
       HashTable[idx] = -1;
       return idx;
   }
   else{
       return -1; // 未找到指定记录，删除失败
   }
}


int main(void)
{
    int i, n, ret;
    int key[] = {19, 14, 23, 1, 68, 20, 84, 27, 55, 11, 10, 79};
    int HashTable[LEN_HT];
    for(i = 0; i < LEN_HT; i++){
        HashTable[i] = -1;
    }

    n = sizeof(key)/sizeof(key[0]);
    for(i = 0; i < n; i++){
        //printf("%d\n", key[i]);
        ret = linear_probing_ins(HashTable, key[i]);
        //printf("%d\n", ret);
    }
    for(i = 0; i < n; i++){
        printf("%d:%d  ", i, HashTable[i]);
    }

    ret = linear_probing_search(HashTable, 27);
    if( ret < 0 ){
        printf("\nNot Found!");
    }
    else{
        printf("\n%d found at HashTable[%d]\n", HashTable[ret], ret);
    }

    ret = linear_probing_del(HashTable, 27);
    if( ret < 0 ){
        printf("\nDeletion Failed!");
    }
    else{
        printf("\nHashTable[%d] was deleted\n", ret);
    }

    ret = linear_probing_search(HashTable, 27);
    if( ret < 0 ){
        printf("\n%d Not Found!", 27);
    }
    else{
        printf("\n%d found at %d\n", HashTable[ret], ret);
    }
    system("pause");
    return 0;
}
