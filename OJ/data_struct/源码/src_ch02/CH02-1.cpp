/*************************************
 *  �̲�P24���㷨2.5  ˳���Ĳ����㷨
 *  ��˳���L�ĵ�pos��λ���ϲ���һ��ֵΪelem����Ԫ��
**************************************/
typedef int ElemType;

int list_insert(ListPtr L, int pos, ElemType e)
{
	int i, len, status = 0;
	len = L->length;
	if (len >= MAXSIZE){ 
		status = 1;      // ˳������������Ԫ�ػᵼ������
		return status;
	}
	else if ((pos<1) || (pos>len+1)){
		status = 1; // ����λ�ò�����
		return status;
	}
	else	{ 
		for(i=len; i>=pos; i--) {
			// ����Ԫ�غ���һ��λ��
			L->elem[i+1] = L->elem[i];
		}
		L->elem[pos] = e;
		L->length ++;
		status = 0;      // Ԫ�ز���ɹ�
	}
	return status;
}