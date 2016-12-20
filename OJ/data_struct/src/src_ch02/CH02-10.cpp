/*******************************************
 *  教材P48：出栈：取出栈顶元素，通过函数参数item返回
********************************************/
# define MAXSIZE 100
typedef int EntryType;

typedef struct stack{
    int top;                                   // 存放栈顶位置
    EntryType entry[MAXSIZE]; // 存放数据元素
}Stack, *StackPtr;

int  stack_push(StackPtr s, EntryType *item)
{
    int status = 0; 
    if( stack_empty(s) ){
        status = -1;      // 如果栈空，则返回失败标识（下溢）
    }
    else{        
        *item = s->entry[s->top]; // 将数据元素从栈顶取出
        (s->top)--;
    }
    return status;
}