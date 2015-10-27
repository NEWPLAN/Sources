#include <stdio.h>
#include <stdlib.h>
/*********************************************
*  P220 �㷨6.5  ���ŵ�ַ��
* ����̽����ɢ�е�ɢ�в��롢ɾ��������㷨
* ����˵����HashTableΪ��ϣ��keyΪ����ؼ���
**********************************************/
#define LEN_HT 16  // ��ϣ��ı�
#define PVAL 13      // ����������ʹ�õĳ���

// ��ϣ����
int hash(int key)
{
    return(key % PVAL);
}

// ����̽����ɢ�У��������
int linear_probing_ins(int HashTable[],int key)
{  int idx, d = 0;
   idx = hash(key);

   while((d < LEN_HT) &&( HashTable[(idx+d)%LEN_HT] != key) && (HashTable[(idx+d)%LEN_HT] != -1)){
      d++;
   }
   //printf("%d:%d\n", key, d);
   if( d == LEN_HT ){
       return -1;   //����,�˳�
   }

   idx = (idx + d) % LEN_HT;
   if( HashTable[idx] <= 0 ){
       // λ��idx��δʹ�ã�ִ�в������
       HashTable[idx] = key;
       return idx;
   }
   else if( HashTable[idx] == key){
       // �������и�Ԫ��
       return idx;
   }
}

// ����̽����ɢ�У����Ҳ���
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

// ����̽����ɢ�У�ɾ������
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
       return -1; // δ�ҵ�ָ����¼��ɾ��ʧ��
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
