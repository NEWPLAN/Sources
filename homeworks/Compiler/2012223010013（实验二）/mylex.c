#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include <ctype.h>

#define MAX_ID_LENGTH	256
#define MAX_TABLE_SIZE 2048
#define SUCCESS         1

/*TOKEN序列*/
typedef enum token
{

    T_0=0,
    T_1=1,
    T_2=2,

    T_INT=3,
    T_IF=4,
    T_ELSE=5,
    T_WHILE=6,
    T_FOR=7,
    T_READ=8,
    T_WRITE=9,
    T_BEGIN=10,
    T_FOUNCTION=11,
    T_END=12,
    T_THEN=13,

    T_ADD=14,
    T_SUB=15,
    T_MUL=16,
    T_DIV=17,
    T_L=18,
    T_LE=19,
    T_G=20,
    T_GE=21,
    T_NE=22,
    T_E=23,
    T_ASSIGN=24,
    T_LPAR=25,
    T_RPAR=26,
    T_COM=27,
    T_SEM=28,

    T_RETURN=29,
    T_ERROR=30,
    T_SYMBOL=31,
    T_CONSTANT=32
} Token;

/*符号表中的结构*/
typedef struct symtableitem
{
    char	name[MAX_ID_LENGTH];
    Token	lexToken;
    int		type;
//	bool	init;
    int     init;/*C89版本编译器不支持BOOL类型，用int代替*/
} SymTableItem;


int line=1;/*全局计数变量，主要用于显示处理所在行的行数*/
SymTableItem symtable[MAX_TABLE_SIZE];

/*保留字*/
char *reserved_table[] =
{
    "integer",
    "if",
    "else",
    "while",
    "for",
    "read",
    "write",
    "begin",
    "function",
    "end",
    "then"
};
/*操作数*/
char* operator_table[]=
{
    "+",
    "-",
    "*",
    "/",
    "<",
    "<=",
    ">",
    ">=",
    "!=",
    "==",
    ":=",
    "(",
    ")",
    ",",
    ";"
};
/*函数声明*/
void InitScanner(void);
int  ScanOneToken(FILE* fp,char* temp);
void deal_one_token(char* temp);
void deal_error(char* temp);
void write_to_lex(char*temp,int count);
void write_to_error(char*temp);
void write_to_symtab1(char*temp,int count);

//************************************
// Method:    InitScanner
// FullName:  InitScanner
// Access:    public
// Returns:   void
// Qualifier:
//************************************
void InitScanner(void)
{
    int count=0;
    for(count=0; count<MAX_TABLE_SIZE; count++)
    {
        symtable[count].lexToken=T_1;
        memset(symtable[count].name,0,MAX_ID_LENGTH);
        symtable[count].init=0;
        symtable[count].type=0;
    }
    system("del lex.txt symtable1.txt error.txt");

    return ;
}

//************************************
// Method:    ScanOneToken
// FullName:  ScanOneToken
// Access:    public
// Returns:   int
// Qualifier:
// Parameter: FILE * fp
// Parameter: char * temp
//************************************
int  ScanOneToken(FILE* fp,char* temp)
{
    int temp_count=0,is_operator=1;
    char ch;
    jmp_buf buf1;
    ch=getc(fp);

    /* 判断是空格和回车跳过*/
    while (ch==' '||ch=='\t'||ch=='\n')
    {
        if(ch=='\n')
            line++;
        ch=getc(fp);
    }

    setjmp(buf1);/*设置还原点，用于数字和字符混合读取*/
    while(isalpha(ch))/*如果是字符*/
    {
        is_operator=0;
        temp[temp_count++]=ch;
        if (temp_count==MAX_ID_LENGTH-1)
        {
            printf("error ....缓冲溢出\n");
            exit(EXIT_FAILURE);
        }
        ch=getc(fp);
    }

    while(isdigit(ch))/*如果是数字*/
    {
        is_operator=0;
        temp[temp_count++]=ch;
        if (temp_count==MAX_ID_LENGTH-1)
        {
            printf("error ....缓冲溢出\n");
            exit(EXIT_FAILURE);
        }
        ch=getc(fp);
        if (isalpha(ch))
        {
            longjmp(buf1,1);/*返回函数的还原点，次目的主要用于字符和数字的混合读取*/
        }
    }
    if (!is_operator)
    {
        ungetc(ch,fp);/*回退一个字符，因为已经识别出一个可以处理的结构*/
        return SUCCESS;
    }


    switch(ch)/*处理非字符和数字情况*/
    {
    case '>':
    case '<':
        temp[temp_count++]=ch;
        ch=getc(fp);
        if (ch=='=')
        {
            temp[temp_count++]=ch;
        }
        else
            ungetc(ch,fp);/*回退一个读进的字符*/
        return SUCCESS;
        break;


    case '!':
    case ':':
    case '=':
        temp[temp_count++]=ch;
        ch=getc(fp);
        if (ch!='=')
        {
            ungetc(ch,fp);
            write_to_error(temp);

            return !SUCCESS;
        }
        else
            temp[temp_count++]=ch;
        return SUCCESS;
        break;


    case '(':
    case ')':
    case ',':
    case ';':
    case '+':
    case '-':
    case '*':
    case '/':
        temp[temp_count++]=ch;
        return SUCCESS;
        break;
    default:
        temp[temp_count++]=ch;
        write_to_error(temp);/*错误处理*/
        system("pause");
        break;
    }
    return !SUCCESS;
}

//************************************
// Method:    deal_one_token
// FullName:  deal_one_token
// Access:    public
// Returns:   void
// Qualifier:
// Parameter: char * temp
//************************************
void deal_one_token(char* temp)
{
    Token temp_token;
    int count=0,help=0,is_digit=0,i=0;

    /*判断是不是关键字，因为关键字是不能作为标示符的*/
    for (count=0; count<11; count++)
    {
        if (!strcmp(temp,reserved_table[count]))
        {
            printf("%s\t\t对应于TOKEN序列： %d\n",temp,count+3);
            write_to_lex(temp,count+3);
            return ;
        }
    }

    /*判断是不是运算符*/
    for (count=0; count<15; count++)
    {
        if (!strcmp(operator_table[count],temp))
        {
            printf("%s\t 存在于运算符中\t对应于TOKEN序列： %d\n",temp,count+14);
            write_to_lex(temp,count+14);
            return ;
        }
    }

    /*查看是不是在符号表中已经标记出来，如果标记则返回，否则在符号表中标记*/
    for(count=0; symtable[count].init&&(count<MAX_TABLE_SIZE); count++)
    {
        if (!strcmp(symtable[count].name,temp))
        {
            printf("%s\t存在于标示符的位置: %d\t对应的TOKEN序列是：%d\n",temp,count,symtable[count].lexToken);
            write_to_lex(temp,symtable[count].lexToken);
            return ;
        }
    }

    if (count==MAX_TABLE_SIZE)
    {
        printf("缓冲区域溢出\n");
        exit(EXIT_FAILURE);
        system("pause");
        return;
    }
    write_to_symtab1(temp,count);/*写入符号表*/
    printf("%s\t对应的TOKEN序列是：%d\n",temp,symtable[count].lexToken);
    write_to_lex(temp,symtable[count].lexToken);
    return ;
}



//************************************
// Method:    write_to_error
// FullName:  write_to_error
// Access:    public
// Returns:   void
// Qualifier: 错误处理
// Parameter: char * temp
//************************************
void write_to_error(char*temp)
{
    FILE *ERROR_FILE=fopen("error.txt","a+");
    if (!ERROR_FILE)
    {
        printf("can`t open file error.txt exit_failure\n");
        system("pause");
        exit(EXIT_FAILURE);
    }
    fprintf(ERROR_FILE,"行%d\t未识别字符组合%s\n",line,temp);
    fclose(ERROR_FILE);
    return;
}

//************************************
// Method:    write_to_lex
// FullName:  write_to_lex
// Access:    public
// Returns:   void
// Qualifier: 写入分析表lex
// Parameter: char * temp
// Parameter: int count
//************************************
void write_to_lex(char*temp,int count)
{
    FILE* LEX_FILE=fopen("lex.txt","a+");
    FILE* GRAMER_FILE=fopen("GRAMER.txt","a+");
    if (!LEX_FILE || !GRAMER_FILE)
    {
        printf("error in open  file lex.txt exit failure\n");
        system("pause");
    }
    fprintf(LEX_FILE,"行%d\t字符：%-10s\t对应TOKEN序列：%d\n",line,temp,count);
    fprintf(GRAMER_FILE,"%d\t%d\n",line,count);
    fclose(LEX_FILE);
    fclose(GRAMER_FILE);
    return;
}

//************************************
// Method:    write_to_symtab1
// FullName:  write_to_symtab1
// Access:    public
// Returns:   void
// Qualifier: 写入符号表
// Parameter: char * temp
// Parameter: int count
//************************************
void write_to_symtab1(char*temp,int count)
{
    int help=0,is_digit=0;
    for (help=0; help<MAX_ID_LENGTH; help++)
    {
        if (temp[help]=='\0')
        {
            break;
        }
        if (isdigit(temp[help]))
        {
            is_digit=1;
        }
        else
        {
            is_digit=0;
            break;
        }
    }

    strcat(symtable[count].name,temp);
    symtable[count].init=1;

    if (is_digit)
    {
        symtable[count].lexToken=T_CONSTANT;
        symtable[count].type=atoi(temp);
    }
    else
    {
        symtable[count].lexToken=T_SYMBOL;
    }
    return;
}

//************************************
// Method:    main
// FullName:  main
// Access:    public
// Returns:   int
// Qualifier: 主函数
// Parameter: int agrc
// Parameter: char * argv[]
//************************************
int main(int agrc,char *argv[])
{
    printf("\t\t2012223010013\n");
    FILE* symtable1=NULL;
    int num=0;
    if(agrc<2)
    {
        printf("ERROR: source file needed.\nUSAGE: mylex source.c\n");
        return 0;
    }
    FILE* fp=fopen(argv[1],"r");
    FILE* file_read=fp;
//	char source_from[20]={0};
    char temp[MAX_ID_LENGTH];

//	strcat(source_from,"e:\\test.txt");
//	file_read=fopen(source_from,"r");
    if(!file_read)
    {
        printf("error in open source\n");
        exit(0);
    }
    InitScanner();
    for (;;)
    {
        if (feof(file_read))
        {
            break;
        }
        memset(temp,0,sizeof(temp));
        ScanOneToken(file_read,temp);
        printf("line= %d\n",line);
        deal_one_token(temp);

    }
    if (!(symtable1=fopen("symtable1.txt","a+")))
    {
        printf("can`t open file symtable1 exit failure\n");
        system("pause");
    }

    /*将符号表输出到文件中去*/
    for (num=0; (num<MAX_TABLE_SIZE)&&(symtable[num].init); num++)
    {
        fprintf(symtable1,"name=%s\tTOKEN=%d,type=%d\n",symtable[num].name,symtable[num].lexToken,symtable[num].type);
    }
    fclose(file_read);
    return 0;
}
