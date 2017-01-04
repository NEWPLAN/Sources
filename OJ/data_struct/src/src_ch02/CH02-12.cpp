/*************************************
 *  �̲�P49-50����ջ�����ݽṹ���������
**************************************/
typedef int EntryType;

/*************************************
 * ������ջ���Ԫ�ص���������
**************************************/
typedef struct node
{   
    EntryType     entry;
    struct  node  *next;
}StackNode, *StackNodePtr;

/*************************************
 * ������ջ����������
**************************************/
Typedef struct stack{      /* ��ջ���Ͷ��� */
    StackNodePtr  top;      /* ָ��ջ����ָ�� */ 
}Stack, *StackPtr;

/*************************************
 * P50���½���ʼ���������Բ���itemΪ��㸳ֵ��
 * ��Ϊÿһ����ջ��������Ҫ�����ɽ��
 * ���Խ������½��Ĳ�����װ�ɺ���
**************************************/
StackNodePtr new_node(EntryType item)
{
	StackNodePtr pNode;
	pNode = (StackNodePtr)malloc(sizeof(StackNode));
	if (pNode){
		pNode->entry = item;
		pNode->next = NULL;
	}
	else{
		printf("�ڴ�ľ�");
		exit(0);
	}
	return pNode;
}

/*************************************
 * P50����ջ�ĳ�ʼ������
**************************************/
int stack_init(StackPtr s)
{
	s->top = NULL;  /* ��ջ��topָ��ֵΪNULL */ 
	return 0;
}

/*************************************
 * P50����ջ����ղ����������٣�
**************************************/
void stack_clear(StackPtr s)
{
	StackNodePtr pNode = s->top;
	while (pNode){
		s->top = pNode->next;
		free(pNode);
		pNode = s->top;
	}
	s->top = NULL; 
}

/*************************************
 * P50����ջ���пղ���
**************************************/
bool stack_empty(StackPtr s)
{
	return s->top == NULL; 
}
