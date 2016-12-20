/*************************************
 *  教材P48：入栈：插入元素item为新的栈顶元素
**************************************/
# define MAXSIZE 100
typedef int EntryType;

typedef struct stack{
    int top;                                   // 存放栈顶位置
    EntryType entry[MAXSIZE]; // 存放数据元素
}Stack, *StackPtr;

int  stack_push(StackPtr s, EntryType item)
{
    int status = 0;   
    if( stack_full(s) ){  // 如果栈满，则返回失败标识（上溢）
        status = -1;
    }
    else{
        s->top++;                          // 首先修改栈顶指针
        s->entry[s->top] = item;  // 将数据元素放入栈顶
    }
    return status;
}