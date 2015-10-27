#define MAX_WORD_LENGTH 31
#include <cstdio>
#include <cstdlib>
#include <malloc.h>
#include <cstring>
#include <iostream>
//����BST���
typedef struct BSTNode
{
    char key[MAX_WORD_LENGTH];/*�洢������*/
    int value;
    BSTNode *lchild;
    BSTNode *rchild;
}BSTNode,*BSTree;
BSTree t=NULL;
int sum = 0;
//��BST������
void insert_tree(char * key)
{
    //xָʾ���ҹ������½���·����yʼ��ָ��x�ĸ����
    BSTree y=NULL;
    BSTree x=t;
    //x�½�
    while(x)
    {      
        y = x;
         if(strcmp(x->key,key) == 0)/*�ҵ��ýڵ㣬Ȩֵ++*/
		 {
              x->value++;
              return;
         }
		 else if(strcmp(x->key, key) >0)/*�����ǰ�ڵ�keyҪ�������������*/
		 {
              x = x->lchild;
         }
		 else
		 {
              x = x->rchild;
         }
    }
     BSTree new_node = (BSTree)malloc(sizeof(BSTNode));
     new_node->lchild = NULL;
     new_node->rchild = NULL;
     strcpy(new_node->key, key);
     new_node->value = 1;
     //�����Ϊ��
    if(t==NULL)
        t=new_node;
    else
    {
        //���ø����ĺ��ӽ��
        if(strcmp(y->key,key) >= 0)
            y->lchild = new_node;
        else
            y->rchild=new_node;
    }
}
void inorder_tree_walk(BSTree x)
{
     if(x !=NULL)
     {
         inorder_tree_walk(x->lchild);
         printf("%s", x->key);
     //   printf("%d\n", x->value);
         printf("\t%.4f\n",(x->value * 100.0) / sum);
         inorder_tree_walk(x->rchild);   
     }
}
int main()
{
     char temp[31];
     while(gets(temp)&& temp[0] != '#')
     {
         insert_tree(temp);
     //   cout << temp << endl;
         sum++;
     }
     inorder_tree_walk(t);
     return 1;
}