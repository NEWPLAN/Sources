/*************************************
 * P50：链栈的入栈操作：插入元素item为新的栈顶元素
**************************************/
int  stack_push(StackPtr s, EntryType item)
{
    int status = 0;
    StackNodePtr pNode = new_node(item); // 新结点初始化
    if (pNode){                          // 内存分配成功
        pNode->next = s->top;   // 将新结点插入到表头位置
        s->top = pNode;              // 修改栈顶指针        
    }
    else {
        status =  1;   // 无法分配存储空间，相当于栈满上溢
    }
    return status;
}