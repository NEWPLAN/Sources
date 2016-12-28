/*************************************
 *  教材P52：括号表达式匹配算法
**************************************/
typedef char EntryType ;

bool  match_brackets(char * expr)
{ 
    StackPtr ps;  
    EntryType item;        // 临时变量，记录当前读入的字符 
    bool match = true;   // 当出现括号不匹配时置为false
    
    stack_init(ps);    /* 构造空栈 */    
    
    // 逐一读取字符串中的字符，对其中的括号进行匹配
    // 方法：遇到左括号则做入栈操作，遇到右括号则与栈顶左括号进行匹配
    while ( match && (item = expr++)!= '\0'){        
        if (item == '(' || item == '[' || item == '{'){
            stack_push(ps, item);   /* 左括号入栈 */
        }
        /* 右括号检查 */
        if (item == ')' || item == ']' || item == '}'){
            if (stack_empty(ps)){
                match = false; 
                printf("括号不匹配：缺乏与%c匹配的左括号", item);
            }
            else{
                StackEntry brack;
                stack_pop(ps, &brack); /* 弹出栈顶左括号 */
                match = ( item ==')' && brack == '(' ) ||
                               ( item =='}' && brack == '{' ) ||
                               ( item ==']' && brack == '[' ) ;
                               
                if (!match){
                    printf("括号不匹配：%c != %c", brack,  item);
                }
            }
        }
    }
    if ( !stack_empty(ps) ){
        match = false; 
        printf("括号不匹配：左括号多于右括号");
    }
    
    stack_destroy(ps);
    return match;
}