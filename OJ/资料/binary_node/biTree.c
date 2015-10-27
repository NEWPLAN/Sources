#include <stdio.h>
#include <stdlib.h>


typedef char EleType;

typedef struct B_Node
{
	EleType data;
	struct B_Node *rchild,*lchild;
}BiNode,*BiTree;

/*用户先序输入二叉树*/
void CreateTree(BiTree * sT)
{
	EleType c;
	scanf("%c",&c);
	if(' '== c)
		*sT=NULL;
	else
	{
		*sT=(BiNode*)malloc(sizeof(BiNode));
		(*sT)->data=c;
		CreateTree(&(*sT)->lchild);
		CreateTree(&(*sT)->rchild);
	}
}
void visit(EleType data,int level)
{
	printf("in level %d is %c\n",level,data);
}
void PreVisitNode(BiTree T,int level)
{
	if(T)
	{
		visit(T->data,level);
		PreVisitNode(T->lchild,level+1);
		PreVisitNode(T->rchild,level+1);
	}
}

int main(int argc ,char** argv)
{
	BiTree head;
	CreateTree(&head);
	PreVisitNode(head,1);
	return 0;
}