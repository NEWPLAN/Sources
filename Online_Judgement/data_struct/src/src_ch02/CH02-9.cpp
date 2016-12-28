/*************************************
 *  �̲�P48����ջ������Ԫ��itemΪ�µ�ջ��Ԫ��
**************************************/
# define MAXSIZE 100
typedef int EntryType;

typedef struct stack{
    int top;                                   // ���ջ��λ��
    EntryType entry[MAXSIZE]; // �������Ԫ��
}Stack, *StackPtr;

int  stack_push(StackPtr s, EntryType item)
{
    int status = 0;   
    if( stack_full(s) ){  // ���ջ�����򷵻�ʧ�ܱ�ʶ�����磩
        status = -1;
    }
    else{
        s->top++;                          // �����޸�ջ��ָ��
        s->entry[s->top] = item;  // ������Ԫ�ط���ջ��
    }
    return status;
}