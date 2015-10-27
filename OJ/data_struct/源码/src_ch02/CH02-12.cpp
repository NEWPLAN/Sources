/*************************************
 *  教材P49-50：链栈的数据结构与基本操作
**************************************/
typedef int EntryType;

/*************************************
 * 定义链栈结点元素的数据类型
**************************************/
typedef struct node
{   
    EntryType     entry;
    struct  node  *next;
}StackNode, *StackNodePtr;

/*************************************
 * 定义链栈的数据类型
**************************************/
Typedef struct stack{      /* 链栈类型定义 */
    StackNodePtr  top;      /* 指向栈顶的指针 */ 
}Stack, *StackPtr;

/*************************************
 * P50：新结点初始化函数（以参数item为结点赋值）
 * 因为每一次入栈操作都需要新生成结点
 * 所以将生成新结点的操作封装成函数
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
		printf("内存耗尽");
		exit(0);
	}
	return pNode;
}

/*************************************
 * P50：链栈的初始化操作
**************************************/
int stack_init(StackPtr s)
{
	s->top = NULL;  /* 空栈的top指针值为NULL */ 
	return 0;
}

/*************************************
 * P50：链栈的清空操作（即销毁）
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
 * P50：链栈的判空操作
**************************************/
bool stack_empty(StackPtr s)
{
	return s->top == NULL; 
}
