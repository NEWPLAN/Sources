/*************************************
 *  教材P26：算法2.6 顺序表的删除算法
 *  删除顺序表L的第pos个位置上的数据元素
**************************************/
typedef int ElemType;

int list_remove(ListPtr L, int pos)
{
	int i, len, status = 0;
	len = L->length;
	
	if ((1 <= pos) && (pos <= len)){
		for (i = pos; i < len; i++){
			L->elem[i] = L->elem[i+1]
		}
		L->length --;
	}
	else {
		status = 1;  // 待删除元素位置越界
	}
	return status;	
}