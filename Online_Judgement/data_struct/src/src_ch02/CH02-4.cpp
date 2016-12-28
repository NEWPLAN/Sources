/*************************************
 *  �̲�P32���㷨2.11  �������н������㷨 
 *  �ڵ�����L�еĵ�pos�����ǰ����ֵΪelem������Ԫ��
 *  ���������½��ռ�ݵ�pos��λ�ã�ԭposλ�ý���Ϊ��pos+1�����
 *  ��������ֵΪ���ͣ�0��ʾ����ɹ���1��ʾ����ʧ��
**************************************/
typedef int ElemType;
int list_getposition(ListPtr L, int pos, ListNodePtr *p);

int list_insert(ListPtr L, int pos, ElemType elem)
{
	int status;
	ListNodePtr prev, pnode;
	
	// Ѱ�Ҳ���λ�õ�ǰ����㣬��prevָ����
	status = list_getposition(L, pos-1, &prev);
	
	if (status) {   // �ɹ��ض�λ������λ��
		pnode = (ListNodePtr) malloc(sizeof(ListNode));
		if (pnode) {
			pnode->data = elem;
			pnode->next = prev->next;
			prev->next = pnode;
		}
		else{
			status = 1;  // Ϊ�½�����ռ�ʧ��
		}
	}
	return status;
}

/*************************************
 *  �̲�P31���㷨2.9  ��������ָ�붨λ�㷨 
 *  �ڵ�����L�в��ҵ�pos����㣬ͨ����������ptr����ָ��ý���ָ��
 *  ��������ֵΪ���ͣ�0��ʾ��λ�ɹ���1��ʾ��λʧ��
**************************************/
// ע��˳���P22�����������ListPtr�Ķ����ǲ�ͬ��
// �̲�P29��typedef ListNodePtr List, *ListPtr;

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