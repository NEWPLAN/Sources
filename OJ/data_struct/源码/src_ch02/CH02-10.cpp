/*******************************************
 *  �̲�P48����ջ��ȡ��ջ��Ԫ�أ�ͨ����������item����
********************************************/
# define MAXSIZE 100
typedef int EntryType;

typedef struct stack{
    int top;                                   // ���ջ��λ��
    EntryType entry[MAXSIZE]; // �������Ԫ��
}Stack, *StackPtr;

int  stack_push(StackPtr s, EntryType *item)
{
    int status = 0; 
    if( stack_empty(s) ){
        status = -1;      // ���ջ�գ��򷵻�ʧ�ܱ�ʶ�����磩
    }
    else{        
        *item = s->entry[s->top]; // ������Ԫ�ش�ջ��ȡ��
        (s->top)--;
    }
    return status;
}