/*
 *self-balance binary sort tree AVL 
 * 提高二叉排序树的效率，防止偏树的发生
 *要求左子树和右子树的高度差<=1,称为平衡因子(balance factor)
 *比如：
 *
 ****a********************************
 *****\*******************************
 ******b******************************
 *******\*****************************
 ********c****************************
 *********\***************************
 **********d**************************
 ***********\*************************
 ************e************************
 *************\***********************
 **************f**********************
 ***************\*********************
 ****************g********************
 *
 */
 
 #define LH 1
 #define EH 0
 #define RH -1
 typedef struct BiTNode
{
	int data;
	int bf;/*balance factor*/
	struct BiTNode *lchild,*rchild;
}BiTNode, *BiTree;

 
 int InsertAVL(BiTree *T,int e,int *taller)
 {
	if(!*T)
	{
		*T=T=(BiTree)malloc(sizeof(BiTNode));
		(*T)->data=e;
		(*T)->lchild=(*T)->rchild=NULL;
		(*T)->bf=EH;
		*taller=true;
	}
	else
	{
		if(e==(*T)->data)
		{
			*taller=false;
			return false;
		}
		if(e<(*T)->data)
		{
			if(!InsertAVL(&(*T)->lchild,e,taller))
				return false;
			if(*taller)
			{
				switch((*T)->bf)
				{
					case LH:/*lef balcance*/
					LeftBalance(T);
					*Taller = false;
					break;
					
					case EH;
					(*T)->bf=LH;
					*taller=true;
					break;
					
					case RH:
					(*T)->bf=EH;
					*taller=false;
					break;
					
				}
			}
		}
		else
		{
			if(!InsertAVL(&(*T)->rchild,e,taller))
				return false;
			if(*taller)
			{
				switch((*T)->bf)
				{
					case LH:/*lef balcance*/
					(*T)->bf=EH;
					*taller=true;
					break;
					
					case EH;
					(*T)->bf=RH;
					*taller=true;
					break;
					
					case RH:
					RightBalance(T);
					*taller=false;
					break;					
				}
		}
	}
 }
 
 void LeftBalance(BiTree *T)
 {
	BiTree L,Lr;
	L=(*T)->lchild;
	
	switch(L->bf)
	{
		case LH:
		(*T)->bf=L->bf=EH;
		R_Rotate(T);
		break;
		
		case RH:
		Lr=L->rchild;
		switch(Lr->bf)
		{
			case LH:
			(*T)->bf=RH;
			L->bf=EH;
			break;
			
			case EH:
			(*T)->bf=L->bf=EH;
			break;
			
			case EH:
			(*T)->bf=EH;
			L->bf=LJ;
			break;
		}
		Lr->bf=EH;
		L_Rotate(&(*T)->lchild);
		R_Rotate(T);
	}
 }
 
 void R_Rotate(BiTree *p)
 {
	BiTree L;
	L=(*p)->lchild;
	(*p)->lchild=L->rchild;
	L->rchild=(*p);
	*p=L;
 } 
 
 void RightBalance(BiTree *T)
 {
	
 }