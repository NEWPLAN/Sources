/*************************************
 *  �̲�P33���㷨2.12  �������н��ɾ���㷨 
 *  ɾ��������L�еĵ�pos���������Ԫ��
 *  ��������ֵΪ���ͣ�0��ʾɾ���ɹ���1��ʾɾ��ʧ��
**************************************/
typedef int ElemType;

int list_remove(ListPtr L, int pos)
{
	int status;
	ListNodePtr ptr, prev;
	
	// Ѱ�Ҵ�ɾ������ǰ����㣬��prevָ����
	status = list_getposition(L, pos-1, &prev);
	
	if (status){
		ptr = prev->next;  // ptrָ���ɾ�����
		// �޸Ĵ�ɾ������ǰ������nextָ��ָ�����̽��
		prev->next = ptr->next; 
		free(ptr);
	}
	return status;
}
