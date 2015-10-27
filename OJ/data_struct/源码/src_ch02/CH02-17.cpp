/*************************************
 *  �̲�P54���������ʽ��ֵ����������ȷ����㷨
 *  �裺OPTR��OPND�ֱ�Ϊ�����ջ��������ջ
**************************************/
typedef int EntryType ;

/*************************************
 * ������ջ���Ԫ�ص���������
**************************************/
typedef struct node
{   
    EntryType     entry;
    struct  node  *next;
}StackNode, *StackNodePtr;

/*************************************
 * ������ջ����������
**************************************/
Typedef struct stack{      /* ��ջ���Ͷ��� */
    StackNodePtr  top;      /* ָ��ջ����ָ�� */ 
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
        if (!is_operand(c)){         // ���c�ǲ�����
            stack_push(OPND, c);  // ��c������ջ����
            expr++;
            c = *expr;
        }
        else{
            switch(prec_compare(OPTR->top->entry, c) ){
                case '<':            // ջ��Ԫ�����ȼ����ڵ�ǰ�����
                    push(OPTR, c);    // ����ǰ�������ջ
                    expr++;
                    c = *expr;            // ������һ���ַ�
                    break;
                case '=':             // ��������
                    pop(OPTR, &op);  // ����������
                    expr++;
                    c = *expr;            // ������һ���ַ�
                    break;
                case '>':            // ջ��Ԫ�����ȼ����ڵ�ǰ�����
                    pop(OPTR, &op);    // �������ջ
                    pop(OPND, &a); pop(OPND, &b);
                    push(OPND, calc(a, b, op));   // ����������ջ
                    break;
            } // end switch
        }
    }// end while
    
    return OPND->top->entry;
}