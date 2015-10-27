/*************************************
 *  �̲�P33���㷨2.13  ������Ĵ����㷨 
 *  �����Ա�n��Ԫ���Ѵ��������elem�У���̬����һ��������L
 *  ��������ֵΪ���ͣ�0��ʾ�����ɹ���1��ʾ����ʧ��
**************************************/
typedef int ElemType;

int list_create(ListPtr L, ElemType elem[], int n)
{
	int i, status = 0;
	ListNodePtr phead, pnode;
	
	// ������ͷ���Ŀձ�
	phead = (ListNodePtr)malloc(sizeof(ListNode));
	phead->next = NULL;
	
	for(i=n-1; i>=0; i--){
		pnode = (ListNodePtr)malloc(sizeof(ListNode));
		if(!pnode){
			status = 1; // Ϊ�½������ռ�ʧ��
			break;
		}
		
		pnode->data = elem[i];  // Ϊ�½�㸳ֵ
		// ���½����뵽��ͷλ�ã�ͷ���֮��
		pnode->next =  phead->next;
		phead->next = pnode;
	}
	*L = phead;
	return status;
}
