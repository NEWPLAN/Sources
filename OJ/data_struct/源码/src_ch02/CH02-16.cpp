/*************************************
 *  �̲�P52�����ű��ʽƥ���㷨
**************************************/
typedef char EntryType ;

bool  match_brackets(char * expr)
{ 
    StackPtr ps;  
    EntryType item;        // ��ʱ��������¼��ǰ������ַ� 
    bool match = true;   // ���������Ų�ƥ��ʱ��Ϊfalse
    
    stack_init(ps);    /* �����ջ */    
    
    // ��һ��ȡ�ַ����е��ַ��������е����Ž���ƥ��
    // ����������������������ջ��������������������ջ�������Ž���ƥ��
    while ( match && (item = expr++)!= '\0'){        
        if (item == '(' || item == '[' || item == '{'){
            stack_push(ps, item);   /* ��������ջ */
        }
        /* �����ż�� */
        if (item == ')' || item == ']' || item == '}'){
            if (stack_empty(ps)){
                match = false; 
                printf("���Ų�ƥ�䣺ȱ����%cƥ���������", item);
            }
            else{
                StackEntry brack;
                stack_pop(ps, &brack); /* ����ջ�������� */
                match = ( item ==')' && brack == '(' ) ||
                               ( item =='}' && brack == '{' ) ||
                               ( item ==']' && brack == '[' ) ;
                               
                if (!match){
                    printf("���Ų�ƥ�䣺%c != %c", brack,  item);
                }
            }
        }
    }
    if ( !stack_empty(ps) ){
        match = false; 
        printf("���Ų�ƥ�䣺�����Ŷ���������");
    }
    
    stack_destroy(ps);
    return match;
}