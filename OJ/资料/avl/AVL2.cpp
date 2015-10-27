#include<stdio.h>
#include<stdlib.h>

#define    OK      1
#define    ERROR   0
#define    TRUE    1
#define    FALSE   0
#define    NULL    0

typedef    int     Status;
typedef    int     ElemType;
typedef    int     KeyType;

typedef struct BSTNode{
	ElemType      data;
	int           bf;         //结点平衡因子
	BSTNode       *lchild, *rchild;   //左右孩子指针
}BSTNode, *BSTree;

Status InitBSTree(BSTree &BT)   //初始化
{//操作结果：构造一个空的动态查找表BT
	BT=NULL;
	return OK;
}//InitBSTree

Status DestroyBSTree(BSTree &BT)     //销毁
{//初始条件：动态表已经存在。操作结果：销毁动态查找表BT
	if(BT)     //非空树
	{
         if(BT->lchild)   //有左孩子
		      DestroyBSTree(BT->lchild);  //递归法销毁左孩子子树
		 if(BT->rchild)   //有右孩子
			  DestroyBSTree(BT->rchild);  //递归法消去右孩子子树
		 free(BT);     //释放根结点
		 BT=NULL;
	}//if
	return OK;
}//DestroyBSTree

BSTree SearchBSTree(BSTree T, KeyType key)  //查找
{//在根指针T所指平衡二叉树中递归地查找某关键字等于key的数据元素
  //若查找成功，则返回指向该数据元素结点的指针，否则返回空指针。
   
	if(!T)
	{
		if(key == T->data)
			return T;     //查找结束；
		else if(key < T->data)
			return SearchBSTree(T->lchild, key);  //在左子树中查找
		else if(key > T->data)
			return SearchBSTree(T->rchild, key);  //在右子树中查找
	}//if
}//SearchBSTree

Status R_Rotate(BSTree &p)   //右旋
{//对以*p为根的二叉树作右旋处理，处理之后p指向新的树根结点，
 //即指向处理前的左子树的根结点（p的左孩子结点）

	BSTree lc;
	lc=p->lchild;     //lc指向左子树的根结点
	p->lchild = lc->rchild;   //lc的右子树挂接为p的左子树
	lc->rchild =p;
	p=lc;      //p指向新的根结点
    return OK;
}//R_Rotate

Status L_Rotate(BSTree &p)   //左旋
{
     BSTree rc;
	 rc=p->rchild;
	 p->rchild = rc->lchild;
	 rc->lchild=p;
	 p=rc;
	 return OK;
}//L_Rotate


#define      LH     +1   //左高
#define      EH     0    //等高
#define      RH     -1   //右高

Status LeftBalance(BSTree &T)  //左平衡旋转处理
{//对以指针T所指结点为根的二叉树作左平衡处理，
 //T指向新根结点
	
	BSTree lc, rd;
	lc=T->lchild;    //lc指向*T的左子树根结点
	switch(lc->bf)
	{//检查*T的左子树的平衡度，并作相应处理

	case LH://新结点插入在*T的左子树上，要做单右旋处理
		T->bf = lc->bf = EH;
		R_Rotate(T);
		break;
	case RH://新结点插入在*T的左孩子的右子树上，要做双旋处理
		rd=lc->rchild; //rd指向*T的左孩子的右子树根
		switch(rd->bf)
		{//修改*T及其左孩子的平衡因子
		case LH: T->bf=RH;
			     lc->bf=EH;
				 break;
		case EH: T->bf=lc->bf=EH;
			     break;
		case RH: T->bf=EH;
			     lc->bf=LH;
		}//switch
		rd->bf=EH;
		L_Rotate(T->lchild);//对*T的左子树做左旋平衡处理
        R_Rotate(T);  //对*T做右旋处理
	}//switch
	return OK;
}//LeftBalance

Status RightBalance(BSTree &T)  //右平衡旋转处理
{//对以T所指向的结点为根二叉树做右平衡处理，
  //T指向新的根结点

	BSTree rc, rd;
	rc=T->rchild;   //rc指向T的右子树根结点
	switch(rc->bf)
	{//检查*T的右子树的平衡度，并作相应平衡处理
	case RH: //新结点插入在右子树右子树上，单向左旋
		T->bf=rc->bf=EH;
		L_Rotate(T);
		break;
	case LH: //新结点插入在右子树的左子树上，双选处理
		rd=rc->lchild;//rd指向右子树的座子树根
		switch(rd->bf)
		{//修改*T及其右孩子的平衡因子
		case RH: T->bf=LH;
			     rc->bf=EH;
				 break;
		case EH: T->bf=rc->bf=EH;
			     break;
		case LH: T->bf=EH;
			     rc->bf=RH;
		}//switch
        rd->bf=EH;
		R_Rotate(T->rchild);//右子树右旋
		L_Rotate(T);    //对*T作左旋平衡处理
	}//switch
	return OK;
}//RightBalance

Status InsertElem(BSTree &T, ElemType e, Status &taller)
{//若不存在e则插入，返回1；否则返回0；若插入后失衡，则作相应平衡处理
 //taller记录树长高与否

	if(!T)
	{//插入新结点树长高，则置taller为TRUE
		T=(BSTree)malloc(sizeof(BSTNode));
		T->data=e;
		T->lchild=T->rchild=NULL;
		T->bf=EH;
		taller=TRUE;
	}
	else
	{
		if(e == T->data)
		{//存在相同结点，则不插入
			taller=FALSE;
			return FALSE;
		}//if
		if(e < T->data)
		{//应继续在T的左子树中搜索
            if(!InsertElem(T->lchild, e, taller))//未插入
				return FALSE;
			if(taller)  //已插入，且左子树长高
			{
                  switch(T->bf)//检查*T的平衡度
				  {
				  case LH://原本左高，需左平衡处理
					  LeftBalance(T);
					  taller=FALSE;
					  break;
				  case EH: //原本等高，左增高则树增高
					    T->bf=LH;
						taller=TRUE;
						break;
				  case RH: //原本右高，则左右等高
					     taller=FALSE;
				  }//switch
			}//if
			else
			{//应在T的右子树中搜索
				if(!InsertElem(T->rchild, e, taller))//未插入
					return FALSE;
				if(taller)
				{
					switch(T->bf)//检查T的平衡度
					{
					case LH: T->bf=EH;
						     taller=FALSE;
							 break;
					case EH: T->bf=RH;
						      taller=TRUE;
							  break;
					case RH: RightBalance(T);
						taller=FALSE;
					}
				}

			}

		}
	}
	return OK;

}
