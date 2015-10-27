/*************************************
 *  教材P54：算术表达式求值的运算符优先分析算法
 *  设：OPTR和OPND分别为运算符栈和运算数栈
**************************************/
typedef int EntryType ;

/*************************************
 * 定义链栈结点元素的数据类型
**************************************/
typedef struct node
{   
    EntryType     entry;
    struct  node  *next;
}StackNode, *StackNodePtr;

/*************************************
 * 定义链栈的数据类型
**************************************/
Typedef struct stack{      /* 链栈类型定义 */
    StackNodePtr  top;      /* 指向栈顶的指针 */ 
}Stack, *StackPtr;

EntryType eval_expr(char * expr)
{
    char op, c = expr[0];
    int a, b;
    StackPtr OPTR, OPND;  
    
    stack_init(OPTR);
    stack_init(OPND);
    stack_push(OPTR, '#');
    
    while(c != '#' ||  OPTR->top->entry  != '#') {
        if (!is_operand(c)){         // 如果c是操作数
            stack_push(OPND, c);  // 对c进行入栈操作
            expr++;
            c = *expr;
        }
        else{
            switch(prec_compare(OPTR->top->entry, c) ){
                case '<':            // 栈顶元素优先级低于当前运算符
                    push(OPTR, c);    // 将当前运算符入栈
                    expr++;
                    c = *expr;            // 读入下一个字符
                    break;
                case '=':             // 处理括号
                    pop(OPTR, &op);  // 弹出左括号
                    expr++;
                    c = *expr;            // 读入下一个字符
                    break;
                case '>':            // 栈顶元素优先级高于当前运算符
                    pop(OPTR, &op);    // 运算符退栈
                    pop(OPND, &a); pop(OPND, &b);
                    push(OPND, calc(a, b, op));   // 将运算结果入栈
                    break;
            } // end switch
        }
    }// end while
    
    return OPND->top->entry;
}