/*************************************
 * P50����ջ����ջ����������Ԫ��itemΪ�µ�ջ��Ԫ��
**************************************/
int  stack_push(StackPtr s, EntryType item)
{
    int status = 0;
    StackNodePtr pNode = new_node(item); // �½���ʼ��
    if (pNode){                          // �ڴ����ɹ�
        pNode->next = s->top;   // ���½����뵽��ͷλ��
        s->top = pNode;              // �޸�ջ��ָ��        
    }
    else {
        status =  1;   // �޷�����洢�ռ䣬�൱��ջ������
    }
    return status;
}