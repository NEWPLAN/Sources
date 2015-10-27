/*************************************
 *  教材P33：算法2.13  单链表的创建算法 
 *  设线性表n个元素已存放在数组elem中，动态创建一个单链表L
 *  函数返回值为整型：0表示创建成功，1表示创建失败
**************************************/
typedef int ElemType;

int list_create(ListPtr L, ElemType elem[], int n)
{
	int i, status = 0;
	ListNodePtr phead, pnode;
	
	// 建立带头结点的空表
	phead = (ListNodePtr)malloc(sizeof(ListNode));
	phead->next = NULL;
	
	for(i=n-1; i>=0; i--){
		pnode = (ListNodePtr)malloc(sizeof(ListNode));
		if(!pnode){
			status = 1; // 为新结点申请空间失败
			break;
		}
		
		pnode->data = elem[i];  // 为新结点赋值
		// 将新结点插入到表头位置（头结点之后）
		pnode->next =  phead->next;
		phead->next = pnode;
	}
	*L = phead;
	return status;
}
