/*********************************************
 *  �̲�P51����ջ�ĳ�ջ����������ջ��Ԫ��(ͨ��ָ�����item����)
**********************************************/
int  stack_pop(StackPtr s, EntryType *item)
{
    int status = 0;
    if (stack_empty(s)){     /* ��ջ�գ���ջ�����ᵼ������ */
        status = 1; 
    }
    else {
        StackNodePtr pNode =  s->top;  // pNodeָ��ջ��Ԫ��
        s->top = pNode->next;               // ջ��ָ�롰�¡���
        *item = pNode->entry;               // ���浱ǰջ��Ԫ��ֵ
        free(pNode);                                // ɾ����ǰջ��Ԫ��
    }
    return status;
}