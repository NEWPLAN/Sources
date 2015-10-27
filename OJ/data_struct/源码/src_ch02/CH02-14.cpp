/*********************************************
 *  教材P51：链栈的出栈操作：弹出栈顶元素(通过指针参数item返回)
**********************************************/
int  stack_pop(StackPtr s, EntryType *item)
{
    int status = 0;
    if (stack_empty(s)){     /* 若栈空，出栈操作会导致下溢 */
        status = 1; 
    }
    else {
        StackNodePtr pNode =  s->top;  // pNode指向栈顶元素
        s->top = pNode->next;               // 栈顶指针“下”移
        *item = pNode->entry;               // 保存当前栈顶元素值
        free(pNode);                                // 删除当前栈顶元素
    }
    return status;
}