/*************************************
 *  教材P32：算法2.11  单链表中结点插入算法 
 *  在单链表L中的第pos个结点前插入值为elem的数据元素
 *  插入结果是新结点占据第pos个位置，原pos位置结点变为第pos+1个结点
 *  函数返回值为整型：0表示插入成功，1表示插入失败
**************************************/
typedef int ElemType;
int list_getposition(ListPtr L, int pos, ListNodePtr *p);

int list_insert(ListPtr L, int pos, ElemType elem)
{
	int status;
	ListNodePtr prev, pnode;
	
	// 寻找插入位置的前驱结点，以prev指向它
	status = list_getposition(L, pos-1, &prev);
	
	if (status) {   // 成功地定位结点插入位置
		pnode = (ListNodePtr) malloc(sizeof(ListNode));
		if (pnode) {
			pnode->data = elem;
			pnode->next = prev->next;
			prev->next = pnode;
		}
		else{
			status = 1;  // 为新结点分配空间失败
		}
	}
	return status;
}

/*************************************
 *  教材P31：算法2.9  单链表中指针定位算法 
 *  在单链表L中查找第pos个结点，通过函数参数ptr返回指向该结点的指针
 *  函数返回值为整型：0表示定位成功，1表示定位失败
**************************************/
// 注意顺序表（P22）和链表关于ListPtr的定义是不同的
// 教材P29：typedef ListNodePtr List, *ListPtr;

int list_getposition(ListPtr L, int pos, ListNodePtr *ptr)
{
	int i = 0, status = 1;
	ListNodePtr p = *L;
	
	while(p && i < pos){
		i++;   p = p->next;
	}
	
	if (p && i ==pos){
		*ptr = p;
		status = 0;
	}
	return status;
}