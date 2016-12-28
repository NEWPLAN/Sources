#define MAX_WORD_LENGTH 31
#include <cstdio>
#include <cstdlib>
#include <malloc.h>
#include <cstring>
#include <iostream>
//定义BST结点
typedef struct BSTNode
{
    char key[MAX_WORD_LENGTH];/*存储？？？*/
    int value;
    BSTNode *lchild;
    BSTNode *rchild;
}BSTNode,*BSTree;
BSTree t=NULL;
int sum = 0;
//向BST插入结点
void insert_tree(char * key)
{
    //x指示查找过程中下降的路径，y始终指向x的父结点
    BSTree y=NULL;
    BSTree x=t;
    //x下降
    while(x)
    {      
        y = x;
         if(strcmp(x->key,key) == 0)/*找到该节点，权值++*/
		 {
              x->value++;
              return;
         }
		 else if(strcmp(x->key, key) >0)/*如果当前节点key要大，则进入左子树*/
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
     //如果树为空
    if(t==NULL)
        t=new_node;
    else
    {
        //设置父结点的孩子结点
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