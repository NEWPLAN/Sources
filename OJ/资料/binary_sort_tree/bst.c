/*
 *binary sort tree
 *�������Ӷ�С�ڸ��ڵ㣬�����к��Ӷ����ڸýڵ�
 *����ݹ�
 *����������Եõ���������
 *delete
 *insert
 *search
*/

typedef struct BiTNode
{
	int data;
	struct BiTNode *lchild,*rchild;
}BiTNode, *BiTree;

/*�ݹ���Ҷ���������
 *f,ָ��T��˫�ף���ʼֵΪNULL
 *�����ҳɹ�����true����pָ��ýڵ㣬���򷵻�false����pָ�����·�������һ���ڵ㣻
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
/*�����������Ĳ��룬�������ڴ�����*/
Status InsertBST(BiTree *T,int key)
{
	BiTree p,s;
	/*p�Ƿ��ʵ����һ���ڵ�*/
	if(!SearchBST(*T,key,NULL,&p))
	{
		s=(BiTree)malloc(sizeof(BiTNode));
		s->data=key;
		s->lchild=s->rchild=NULL;
		
		if(!p) /*pΪ�յ�ʱ��*/
			*T=s; /*����sΪ�µĸ��ڵ�*/
		else if(key<p->data)
			p->lchild=s; /*����sΪ����*/
		else
			p->rchild=s;/*����sΪ�Һ���*/
		return true;			
	}
	else/*�Ѿ���������*/
		return false;
}


/*����������ɾ��
 *�����������
 *1:��ɾ���ڵ�ΪҶ�ڵ㣬ֱ��ɾ���Ϳ��ԣ������bst���ʸı�
 *2�������ɾ���ڵ����һ����֧����ɾ�����ѻ��ڽڵ����õط�����
 *3��������������Ҳ������������������������򼴿ɣ�
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
		(*p)->data=s->data;/*�滻data����*/
		if(q!=(*p)) /*���qΪp�ĵ�ǰ�ڵ㣬���޸�q����ڵ㣬����Ϊ�޸�q���ҽڵ�*/
			q->rchild=s->lchild;
		else
			q->lchild=s->lchild;
		free(s);
	}
 }