/*************************************
 *  �̲�P26���㷨2.6 ˳����ɾ���㷨
 *  ɾ��˳���L�ĵ�pos��λ���ϵ�����Ԫ��
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
		status = 1;  // ��ɾ��Ԫ��λ��Խ��
	}
	return status;	
}