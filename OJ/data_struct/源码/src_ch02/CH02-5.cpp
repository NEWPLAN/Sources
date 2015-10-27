/*************************************
 *  教材P33：算法2.12  单链表中结点删除算法 
 *  删除单链表L中的第pos个结点数据元素
 *  函数返回值为整型：0表示删除成功，1表示删除失败
**************************************/
typedef int ElemType;

int list_remove(ListPtr L, int pos)
{
	int status;
	ListNodePtr ptr, prev;
	
	// 寻找待删除结点的前驱结点，以prev指向它
	status = list_getposition(L, pos-1, &prev);
	
	if (status){
		ptr = prev->next;  // ptr指向待删除结点
		// 修改待删除结点的前驱结点的next指针指向其后继结点
		prev->next = ptr->next; 
		free(ptr);
	}
	return status;
}
