#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ID_LENGTH	256
#define MAX_TABLE_SIZE 2048
#define SUCCESS         1

typedef enum token
{

    T_0=0,          T_1=1,          T_2=2,          T_INT=3,            T_IF=4,         T_ELSE=5,
    T_WHILE=6,      T_FOR=7,        T_READ=8,       T_WRITE=9,          T_BEGIN=10,     T_FOUNCTION=11,
    T_END=12,       T_THEN=13,      T_ADD=14,       T_SUB=15,           T_MUL=16,       T_DIV=17,
    T_L=18,         T_LE=19,        T_G=20,         T_GE=21,            T_NE=22,        T_E=23,
    T_ASSIGN=24,    T_LPAR=25,      T_RPAR=26,      T_COM=27,           T_SEM=28,       T_RETURN=29,
    T_ERROR=30,     T_SYMBOL=31,    T_CONSTANT=32
} Token;

typedef struct LEX
{
    Token read;
    struct LEX *next;
    size_t line;
} MYLEX;


MYLEX lex_head,error_temp;
MYLEX* current;
Token error_TOKEN=T_0;
/*初始化信息*/

/*函数申明*/
int item(void);
int Block(void);
int Factor(void);
int Variable(void);
int Function(void);
int Constant(void);
int Parameter (void );
int Identifier (void);
int Main_Program(void);
int Function_call(void);
void emergency_exit(void);
int Read_statement(void);
int Write_statement(void);
int Execute_statement(void);
MYLEX* Advance(MYLEX* read);
MYLEX* Read_file(MYLEX* read);
int Relational_operators(void);
int Assignment_statement(void);
int Variable_declaration(void);
int Function_declaration(void);
int match(Token read,Token aim);
int Conditional_statement(void);
int Arithmetic_expression(void);
int Declarative_statement(void);
int Conditional_expression(void);
int Execute_statement_table(void);
int error(MYLEX *current,Token aim);
int Declarative_statement_table(void);


/*退出*/
void emergency_exit(void)
{
    MYLEX* EXIT_TEEMP=lex_head.next;
    current=EXIT_TEEMP;
    while(!current)
    {
        current=EXIT_TEEMP->next;
        free(EXIT_TEEMP);
    }
    exit(0);
}

/*错误处理*/
int error(MYLEX *current,Token aim)
{
    FILE *error_file=fopen("error.txt","a");
    if(!error_file)
    {
        perror("can`t open file \n");
    }
    if(error_temp.line<current->line)
    {
        error_temp=*current;
        error_TOKEN=aim;
        fprintf(error_file,"error in line:%d\tTOKEN:%d\texpeced Token is :%d\n",
                error_temp.line,error_temp.read,error_TOKEN);
    }
    fclose(error_file);
    return SUCCESS;
}
//************************************
// Method:    Read_file
// FullName:  Read_file
// Access:    public
// Returns:   int
// Qualifier:读文件
// Parameter: void
//************************************
MYLEX* Read_file(MYLEX* current)
{
    current->line=0;
    current->next=NULL;
    current->read=T_0;

    MYLEX* Temp;
    Temp=current;
    FILE* READ_FILE=fopen("GRAMER.txt","r");
    if(!READ_FILE)
    {
        printf("can`t load file GRAMER.txt, a file like GRAMER.txt is expectd here \n");
        exit(0);
    }
    while(!feof(READ_FILE))
    {
        Temp->next=(MYLEX*)malloc(sizeof(MYLEX));
        Temp=Temp->next;
        if(!Temp)
        {
            printf("can`t malloc a space for this program!\n");
            emergency_exit();
        }
        Temp->line=0;
        Temp->next=NULL;
        Temp->read=T_0;
        fscanf(READ_FILE,"%d\t%d\n",&(Temp->line),&(Temp->read));
    }
    fclose(READ_FILE);
    return current->next;
}

//************************************
// Method:    Advance
// FullName:  Advance
// Access:    public
// Returns:   Token
// Qualifier:	前进移向下一个TOKEN
// Parameter: void
//************************************
MYLEX* Advance(MYLEX* current)
{
    FILE*EAT_FILE=fopen("symtab2.txt","a");
    FILE* LEFT_FILE=fopen("detail.txt","a");
    if((!EAT_FILE)||(!LEFT_FILE))
    {
        perror("can`t open file");
        emergency_exit();
    }
    MYLEX* Temp=current->next;
    printf("you have eat the one is :%3d\n\n follow is \n",current->read);
    fprintf(EAT_FILE,
            "\nyou have eat the one is line:%3d\tToken:%3d\n",
            current->line,current->read);
    fprintf(LEFT_FILE,
            "\n\ncurrent eat the one is line:%3d\tToken:%3d\nfollow is\n",
            current->line,current->read);
    for(Temp; Temp!=NULL; Temp=Temp->next)
    {
        printf("line:%3d \t token=%3d\n",
               Temp->line,Temp->read);
        fprintf(LEFT_FILE,"line:%3d \t token=%3d\n",
                Temp->line,Temp->read);
    }
    fclose(EAT_FILE);
    fclose(LEFT_FILE);
    if(current==NULL)
    {
        printf("all work is to be done!,there is an error in your project!\n ");
        emergency_exit();
    }
    return current->next;
}

//************************************
// Method:    main
// FullName:  main
// Access:    public
// Returns:   int
// Qualifier:	主程序
// Parameter: int agrc
// Parameter: char * agrv[]
//************************************
int main(int agrc, char*agrv[])
{
    error_temp.line=0;
    error_temp.next=NULL;
    error_temp.read=T_0;

    printf("\t编译原理实验二\n\t2012223010013\n\t\t—NEWPLAN\n");
    current=Read_file(&lex_head);
    system("pause");

    if(Main_Program())
    {
        printf("\n\ncongratulation\tsuccessfully!\n\n");
        system("del error.txt");
    }
    else
    {
        printf(" sorry you have failed!\n");
        printf("error in line:%d\tTOKEN:%d\texpeced Token is :%d\n",
               error_temp.line,error_temp.read,error_TOKEN);
    }
    system("pause");
    emergency_exit();/*释放空间，函数返回*/
    return 0;
}

//************************************
// Method:    match
// FullName:  match
// Access:    public
// Returns:   int
// Qualifier:	匹配
// Parameter: Token read
// Parameter: Token aim
//************************************
int match(Token read ,Token aim)
{
    return (read==aim)? (current=Advance(current),SUCCESS): (error(current,aim),!SUCCESS);
}

//************************************
// Method:    Main_Program
// FullName:  Main_Program
// Access:    public
// Returns:   int
// Qualifier:	开始主程序
// Parameter: void
//************************************
int Main_Program(void)
{
    return Block()? SUCCESS: !SUCCESS;
}

//************************************
// Method:    Block
// FullName:  Block
// Access:    public
// Returns:   int
// Qualifier:/*分程序*/
// Parameter: Token read
//************************************
int Block(void)
{
    MYLEX* Temp=current;
    return (match(current->read,T_BEGIN)&&Declarative_statement_table()&&match(current->read,T_SEM)&&
            Execute_statement_table()&&match(current->read,T_END))?SUCCESS:(current=Temp,!SUCCESS);
}


//************************************
// Method:    Declarative_statement_table
// FullName:  Declarative_statement_table
// Access:    public
// Returns:   int
// Qualifier:/*说明语句表*/
// Parameter: Token read
//************************************
int Declarative_statement_table()
{
    MYLEX* Temp=current;
    if(!Declarative_statement())
    {
        current=Temp;
        return !SUCCESS;
    }
    /*采用扩展的BNF方法消除左递归*/
    for(;;)
    {
        Temp=current;
        if(!match(current->read,T_SEM))
            break;
        if(!Declarative_statement())
        {
            current=Temp;
            break;
        }
    }
    return SUCCESS;
}


//************************************
// Method:    Declarative_statement
// FullName:  Declarative_statement
// Access:    public
// Returns:   int
// Qualifier:/*说明语句*/
// Parameter: Token read
//************************************
int Declarative_statement(void)
{
    MYLEX* Temp=current;
    return Variable_declaration()? SUCCESS :
           (current=Temp,Function_declaration()? SUCCESS:!SUCCESS);
}


//************************************
// Method:    Execute_statement_table
// FullName:  Execute_statement_table
// Access:    public
// Returns:   int
// Qualifier:/*执行语句表*/
// Parameter: Token read
//************************************
int Execute_statement_table(void)
{
    MYLEX* Temp=current;
    if(!Execute_statement())
    {
        current=Temp;
        return !SUCCESS;
    }
    for(;;)
    {
        Temp=current;
        if(!match(current->read,T_SEM))
            break;
        if(!Execute_statement())
        {
            current=Temp;
            break;
        }
    }
    return SUCCESS;
}


//************************************
// Method:    Execute_statement
// FullName:  Execute_statement
// Access:    public
// Returns:   int
// Qualifier:/*执行语句*/
// Parameter: Token read
//************************************
int Execute_statement(void)
{
    MYLEX* Temp=current;
    if(Read_statement())
        return SUCCESS;
    current=Temp;
    if(Write_statement())
        return SUCCESS;
    current=Temp;
    if(Conditional_statement())
        return SUCCESS;
    current=Temp;
    if(Assignment_statement())
        return SUCCESS;
    current=Temp;
    return !SUCCESS;
}


//************************************
// Method:    Variable_declaration
// FullName:  Variable_declaration
// Access:    public
// Returns:   int
// Qualifier:/*变量说明*/
// Parameter: Token read
//************************************
int Variable_declaration(void)
{
    MYLEX* Temp=current;
    return (match(current->read , T_INT)&&Variable())?SUCCESS:(current=Temp,!SUCCESS);
}


//************************************
// Method:    Function_declaration
// FullName:  Function_declaration
// Access:    public
// Returns:   int
// Qualifier:/*函数说明*/
// Parameter: Token read
//************************************
int Function_declaration(void)
{
    MYLEX* Temp=current;
    return (match(current->read, T_INT)&&match(current->read, T_FOUNCTION)&&Identifier()&&
            match(current->read, T_LPAR)&&Parameter()&&match(current->read, T_RPAR)&&
            match(current->read,T_SEM)&&Function())? SUCCESS:(current=Temp,!SUCCESS);
}


//************************************
// Method:    Variable
// FullName:  Variable
// Access:    public
// Returns:   int
// Qualifier:/*变量*/
// Parameter: Token read
//************************************
int Variable(void)
{
    return match(current->read,T_SYMBOL)?SUCCESS:!SUCCESS;
}

//************************************
// Method:    Identifier
// FullName:  Identifier
// Access:    public
// Returns:   int
// Qualifier:/*标识符*/
// Parameter: Token read
//************************************
int Identifier  (void)
{
    return match(current->read,T_SYMBOL)?SUCCESS:!SUCCESS;
}


//************************************
// Method:    Parameter
// FullName:  Parameter
// Access:    public
// Returns:   int
// Qualifier:/*参数*/
// Parameter: Token read
//************************************
int Parameter  (void)
{
    MYLEX* Temp=current;
    return Variable()?SUCCESS:(current=Temp,!SUCCESS);
}


//************************************
// Method:    Function
// FullName:  Function
// Access:    public
// Returns:   int
// Qualifier:/*函数体*/
// Parameter: Token read
//************************************
int Function(void)
{
    MYLEX* Temp=current;
    return (match(current->read, T_BEGIN)&&Declarative_statement_table()&&match(current->read, T_SEM)&&
            Execute_statement_table()&&match(current->read,T_END))?SUCCESS:(current=Temp,!SUCCESS);
}


//************************************
// Method:    Read_statement
// FullName:  Read_statement
// Access:    public
// Returns:   int
// Qualifier:/*读语句*/
// Parameter: Token read
//************************************
int Read_statement(void)
{
    MYLEX* Temp=current;
    return (match(current->read,T_READ)&&match(current->read,T_LPAR)&&Variable()&&
            match(current->read,T_RPAR))?SUCCESS:(current=Temp,!SUCCESS);
}


//************************************
// Method:    Write_statement
// FullName:  Write_statement
// Access:    public
// Returns:   int
// Qualifier:/*写语句*/
// Parameter: Token read
//************************************
int Write_statement(void)
{
    MYLEX* Temp=current;
    return (match(current->read,T_WRITE)&&match(current->read,T_LPAR)&&Variable()&&
            match(current->read,T_RPAR))?SUCCESS:(current=Temp,!SUCCESS);
}


//************************************
// Method:    Assignment_statement
// FullName:  Assignment_statement
// Access:    public
// Returns:   int
// Qualifier:/*赋值语句*/
// Parameter: Token read
//************************************
int Assignment_statement(void)
{
    MYLEX* Temp=current;
    return (Variable()&&match(current->read,T_ASSIGN)&&
            Arithmetic_expression())? SUCCESS:(current=Temp,!SUCCESS);
}

//************************************
// Method:    Conditional_statement
// FullName:  Conditional_statement
// Access:    public
// Returns:   int
// Qualifier:/*条件语句*/
// Parameter: Token read
//************************************
int Conditional_statement(void)
{
    MYLEX* Temp=current;
    return (match(current->read,T_IF)&&Conditional_expression()&&
            match(current->read,T_THEN)&&Execute_statement()&&
            match(current->read,T_ELSE)&&Execute_statement()
           )?SUCCESS:(current=Temp,!SUCCESS);
}

//************************************
// Method:    Arithmetic_expression
// FullName:  Arithmetic_expression
// Access:    public
// Returns:   int
// Qualifier:/*算术表达式*/
// Parameter: Token read
//************************************
int Arithmetic_expression(void)
{
    MYLEX* Temp=current;
    if(!item())
    {
        current=Temp;
        return !SUCCESS;
    }
    for(;;)
    {
        Temp=current;
        if(!match(current->read,T_SUB))
            break;
        if(!item())
        {
            current=Temp;
            break;
        }
    }
    return SUCCESS;
}

//************************************
// Method:    item
// FullName:  item
// Access:    public
// Returns:   int
// Qualifier:/*项*/
// Parameter: Token read
//************************************
int item(void)
{
    MYLEX* Temp=current;
    if(!Factor())
    {
        current=Temp;
        return !SUCCESS;
    }
    for(;;)
    {
        Temp=current;
        if(!match(current->read,T_MUL))
            break;
        if(!Factor())
        {
            current=Temp;
            break;
        }
    }
    return SUCCESS;
}

//************************************
// Method:    Constant
// FullName:  Constant
// Access:    public
// Returns:   int
// Qualifier:/*常数*/
// Parameter: Token read
//************************************
int Constant(void)
{
    return match(current->read,T_CONSTANT)?SUCCESS: !SUCCESS;
}

//************************************
// Method:    Conditional_expression
// FullName:  Conditional_expression
// Access:    public
// Returns:   int
// Qualifier:/*条件表达式*/
// Parameter: Token read
//************************************
int Conditional_expression(void)
{
    MYLEX* Temp=current;
    return (Arithmetic_expression()&&Relational_operators()&&
            Arithmetic_expression())? SUCCESS:(current=Temp,!SUCCESS);
}

//************************************
// Method:    Relational_operators
// FullName:  Relational_operators
// Access:    public
// Returns:   int
// Qualifier:/*关系运算符*/
// Parameter: Token read
//************************************
int Relational_operators(void)
{
    return ( match(current->read,T_L)||match(current->read,T_LE)||match(current->read,T_G)||
             match(current->read,T_GE)||match(current->read,T_E)||match(current->read,T_NE)
           )?SUCCESS:!SUCCESS;

}


//************************************
// Method:    Factor
// FullName:  Factor
// Access:    public
// Returns:   int
// Qualifier:/*因子*/
// Parameter: Token read
//************************************
int Factor(void)
{
    MYLEX* Temp=current;
    if(Function_call())
        return SUCCESS;
    current=Temp;
    if(Variable())
        return SUCCESS;
    current=Temp;
    if(Constant())
        return SUCCESS;
    current=Temp;
    return !SUCCESS;
}

//************************************
// Method:    Function_call
// FullName:  Function_call
// Access:    public
// Returns:   int
// Qualifier:/*函数调用*/
// Parameter: Token read
//************************************
int Function_call(void)
{
    MYLEX* Temp=current;
    return (Identifier()&&match(current->read,T_LPAR)&&Arithmetic_expression()&&
            match(current->read,T_RPAR))?SUCCESS:(current=Temp,!SUCCESS);
}
