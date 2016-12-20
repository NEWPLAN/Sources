/*************************************
 *  教材P24：算法2.5  顺序表的插入算法
 *  在顺序表L的第pos个位置上插入一个值为elem的新元素
**************************************/
typedef int ElemType;

int list_insert(ListPtr L, int pos, ElemType e)
{
	int i, len, status = 0;
	len = L->length;
	if (len >= MAXSIZE){ 
		status = 1;      // 顺序表表满，插入元素会导致上溢
		return status;
	}
	else if ((pos<1) || (pos>len+1)){
		status = 1; // 插入位置不合理
		return status;
	}
	else	{ 
		for(i=len; i>=pos; i--) {
			// 数据元素后移一个位置
			L->elem[i+1] = L->elem[i];
		}
		L->elem[pos] = e;
		L->length ++;
		status = 0;      // 元素插入成功
	}
	return status;
}