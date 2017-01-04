/*
 *binary sort tree
 *所有左孩子都小于根节点，所有有孩子都大于该节点
 *满足递归
 *中序遍历可以得到递增排序
 *delete
 *insert
 *search
*/

typedef struct BiTNode
{
	int data;
	struct BiTNode *lchild,*rchild;
}BiTNode, *BiTree;

/*递归查找二叉排序树
 *f,指向T的双亲，初始值为NULL
 *若查找成功返回true，且p指向该节点，否则返回false，且p指向访问路径的最后一个节点；
 */
Status SearchBST(BiTree T,int key ,BiTree f,BiTree *p)
{
	if(!T)
	{
		*p=f;
		return false;
	}
	else if(key==T->data)
	{
		*p=T;
		return true;
	}
	else if(key<T->data)
		return SearchBST(T->lchild,key,T,p);
	else
		return SearchBST(T->rchild,key,T,p);
}
/*二叉排序树的插入，可以用于创建树*/
Status InsertBST(BiTree *T,int key)
{
	BiTree p,s;
	/*p是访问的最后一个节点*/
	if(!SearchBST(*T,key,NULL,&p))
	{
		s=(BiTree)malloc(sizeof(BiTNode));
		s->data=key;
		s->lchild=s->rchild=NULL;
		
		if(!p) /*p为空的时候*/
			*T=s; /*插入s为新的跟节点*/
		else if(key<p->data)
			p->lchild=s; /*插入s为左孩子*/
		else
			p->rchild=s;/*插入s为右孩子*/
		return true;			
	}
	else/*已经存在树中*/
		return false;
}


/*二叉排序树删除
 *包含以下情况
 *1:待删除节点为叶节点，直接删除就可以，不会对bst性质改变
 *2：如果待删除节点仅有一个分支，则删除，把还在节点接入该地方即可
 *3：既有左子树，也有右子树（根据中序遍历规则即可）
 */
 Status DeleteBST(BiTree *T,int key)
 {
	if(!*T)
		return false;
	if(key==(*T)->data)
		return Delete(T);
	else if(key<(*T)->data)
		return DeleteBST(&(*T)->lchild,key);
	else
		return DeleteBST(&(*T)->rchild,key);
	 
 }
 
 Status Delete(BiTree *p)
 {
	BiTree q,s;
	
	if((*p)->rchild==NULL)
	{
		q=*p;
		*p=(*p)->lchild;
		free(q);
	}
	else if((*p)->lchild==NULL)
	{
		q=*p;
		*p=(*p)->rchild;
		free(q);
	}
	else
	{
		q=(*p);
		s=(*p)->lchild;
		while(s->rchild)
		{
			q=s;
			s=s->rchild;
		}
		(*p)->data=s->data;/*替换data部分*/
		if(q!=(*p)) /*如果q为p的当前节点，则修改q的左节点，否则为修改q的右节点*/
			q->rchild=s->lchild;
		else
			q->lchild=s->lchild;
		free(s);
	}
 }