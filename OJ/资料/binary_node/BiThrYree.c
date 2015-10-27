#include <stdio.h>
#include <stdlib.h>

typedef char ElemType;

/*
线索存储标志位：
Link(0)表示左右孩子的指针
Thread(1)表示指向前驱后继的线索
*/
typedef enum{Link, Thread}PointerTag;

typedef struct BiThrNode
{
	ElemType data;
	struct BiThrNode* lchild,* rchild;
	PointerTag lTag,rTag;
}BiThrNode, *BiThrTree;


/*全局变量，始终指向刚刚访问的节点*/
BiThrTree pre;

/*创建一颗二叉树，约定用户遵循谦虚遍历的方式输入数据*/

void CreateTree(BiThrTree * T)
{
	ElemType c;
	scanf("%c",&c);
	if(' '==c)
		*T=NULL;
	else
	{
		*T=(BiThrNode*)malloc(sizeof(BiThrNode));
		(*T)->data=c;
		(*T)->lTag=Link;
		(*T)->rTag=Link;
		CreateTree(&(*T)->lchild);
		CreateTree(&(*T)->rchild);
	}
}
void Visit(ElemType data,int level)
{
	printf("in level %d is data %c\n",level,data);
}
/*中序遍历线索化*/
void MidVisit(BiThrTree T,int level)
{
	if(T)
	{
		MidVisit(T->lchild,level+1);/*递归左孩子线索化*/
		Visit(T->data,level);
		
		if(! T->lchild)/*如果该节点没有做孩子，设置ltag为thread,并把Lchild指向刚刚访问的节点*/
		{
			T->lTag=Thread;
			T->lchild=pre;
		}
		if(!pre->rchild)
		{
			pre->rTag=Thread;
			pre->rchild=T;
		}
		
		pre=T;
		MidVisit(T->rchild,level+1);/*递归左孩子线索化*/
	}
	return ;
}

void InOrderThreadTag(BiThrTree *p,BiThrTree T)
{
	*p=(BiThrTree)malloc(sizeof(BiThrNode));
	(*p)->lTag=Link;
	(*p)->rTag=Thread;
	(*p)->rchild=*p;
	
	if(!T)
		(*p)->lchild=*p;
	else
	{
		(*p)->lchild=T;
		pre=*p;
		MidVisit(T,1);
		
		
		pre->rchild=*p;
		pre->rTag=Thread;
		(*p)->rchild=pre;
	}
}

int main(int argc,char** argv)
{
	BiThrTree p, head=NULL;
	CreateTree(&head);
	//MidVisit(head,1);
	InOrderThreadTag(&p,head);
	return 0;
}