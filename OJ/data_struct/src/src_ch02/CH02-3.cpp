/*************************************
 *  教材P31：算法2.8  单链表中按值查找算法
 *  在单链表L中查找是否存在值为elem的数据元素
 *  采用指针类型参数pos获取待查元素的位置信息（返回给调用函数）
 *  函数返回值为整型：0表示查找成功，1表示查找失败
**************************************/
typedef int ElemType;

int list_locate(ListPtr L, ElemType elem, int *pos)
{
	int i=1, status = 1;
	
	// 指针p指向链表L的第一个元素结点
	ListNodePtr p = L->next;
	
	// 沿着链表结点的链接顺序逐一访问链表元素
	while(p){ // 当p指向表尾结点时，最后一次执行循环体
		if(p->data == elem) {
			break;    // 找到指定元素
		}
		i++;  // 计数器，用于获取待查元素的编号（索引）
		p = p->next;
	}
	// 体会：上述语句可以合并写为：
	// while(p && p->data != elem ) p = p->next;
	
	if (p){
		*pos = i;     // 通过地址指针修改主调函数中的变量值
		status = 0;  // 查找成功
	}
	return status;
}
