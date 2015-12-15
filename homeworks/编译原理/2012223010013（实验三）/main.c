#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#ifndef success
#define success 1
#endif

#define MAX_AMOUNT 20

/*������ս��*/
typedef struct NOterminal
{
    char character;
    int  first_number;/*FIRST�����ж�����ʼΪ0*/
    int  follow_number;/*FOLLOW�����ж�����ʼΪ0*/
    char * FIRST;/*FIRST ����*/
    char * FOLLOW;/*FOLLOW����*/
    struct NOterminal *next;
} noterminal;

/*�����ս��*/
typedef struct Terminal
{
    char  character;/*��ǰ���ַ�*/
    struct Terminal *next;
} terminal;

/*�������ʽ*/
typedef struct PRODUCTION
{
    char source;/*�����Ŀ�ʼ*/
    char * result;/*�����Ľ��*/
    struct PRODUCTION *next;/*ָ����һ��`*/
} production;

int amount=0;
char TEST_STACK[20];
char* TEST_LIST[10];
char terminal_all[10]= {0};
terminal   TERMINAL_HEAD,   *current_terminal=&TERMINAL_HEAD;
noterminal NOTERMINAL_HEAD, *current_noterminal=&NOTERMINAL_HEAD;
production PRODUCTION_HEAD, *current_production=&PRODUCTION_HEAD;

/*��������*/
size_t read(void);
size_t test(void);
size_t write(void);
void Test_read(void);
size_t STACK_FULL();
void STACK_POP(void);
size_t STACK_EMPTY();
void init_stack(void);
void prediction(void);
void test_follow(void);
void emergency(int model);
void prediction_table(void);
void STACK_PUSH(char source);
void insert_to_terminal(char get);
void insert_to_noterminal(char get);
void eliminate_left_recursion(void);
void combine(char* destinction,char* source);
size_t is_appeared(char tobejudged,char*source);
void insert_to_production(char source , char* result);
size_t find_first(noterminal* this_noterminal,production *this_production);
size_t find_follow(noterminal* this_noterminal,production *this_production);



int main(void)
{
    TERMINAL_HEAD.next=NULL;
    NOTERMINAL_HEAD.next=NULL;
    PRODUCTION_HEAD.next=NULL;
    read();
    Test_read();
    printf("\n������ݹ�\n\n");
    eliminate_left_recursion();

    Test_read();

    printf("\n��FIRST��\n\n");
    prediction();
    printf("\n��FOLLOW��\n\n");
    test_follow();
    prediction_table();
    emergency(0);
    return 0;
}

/*���ļ�*/
size_t read(void)
{
    int line=0,model=0,old_line=0;
    int number=0;
    char current_get=0,read_temp[10]= {0};
    FILE * read_stream=fopen("test1.txt","r");
    if(!read_stream)
    {
        printf("error in open file ,can`t open file\n");
        exit(EXIT_FAILURE);
    }
    insert_to_terminal('#');/*����ջ��Ԫ�أ���#��ʾջ��*/
    insert_to_terminal('^');/*����մ�Ԫ�أ���^��ʾջ��*/
    while(!feof(read_stream))
    {
        current_get=fgetc(read_stream);
        while(current_get==' ')
            current_get=fgetc(read_stream);/*���Կո�*/
        while(current_get=='\n')
        {
            current_get=fgetc(read_stream);
            line++;/*��������*/
        }

        switch(line)
        {
        case 0:
            insert_to_noterminal(current_get);
            break;
        case 1:
            insert_to_terminal(current_get);
            break;
        case 3:
            ungetc(current_get,read_stream);
            old_line=1;
            break;
        default:
            perror("error in format of program \n");
            emergency(0);
            break;
        }
        if(old_line)
            break;
    }
    while(!feof(read_stream))
    {
        memset(read_temp,0,sizeof(read_temp));
        old_line=line;
        current_get=fgetc(read_stream);
        while(current_get==' ')
            current_get=fgetc(read_stream);/*���Կո�*/
        while(current_get=='\n')
        {
            current_get=fgetc(read_stream);
            line++;/*��������*/
        }
        model=((line-old_line)>model)? (line-old_line): model;
        switch(model)
        {
        case 0:
        case 1:
            fscanf(read_stream,"%s",read_temp);
            insert_to_production(current_get,read_temp);
            break;
        case 2:
            ungetc(current_get,read_stream);
            TEST_LIST[number]=(char*)malloc(20);
            memset(TEST_LIST[number],0,20);
            fscanf(read_stream,"%s",TEST_LIST[number++]);
            break;
        default:
            perror("error in format of program \n");
            emergency(1);
            break;
        }
    }

    fclose(read_stream);
    return success;
}
/*����*/
size_t test(void)
{
    noterminal *TEMP_NOTERMINAL=NOTERMINAL_HEAD.next;

    for(; TEMP_NOTERMINAL!=NULL; TEMP_NOTERMINAL=TEMP_NOTERMINAL->next)
    {
        printf("%c\tfirst number=%d\tfirst=%s\n",
               TEMP_NOTERMINAL->character,
               TEMP_NOTERMINAL->first_number,
               TEMP_NOTERMINAL->FIRST);
    }
    printf("\n");
    return success;
}

/*��FIRST����*/
size_t find_first(noterminal* this_noterminal,production *this_production)
{
    noterminal* this_noterminal_temp;
    char temp[2]= {0};
    char *TEMP2,*help;
    while(this_production->source!=this_noterminal->character)
        this_production=this_production->next;/*������һ������ʽ*/

    /*�鿴�Ƿ��ǵ�һ�Σ�����ǣ�����ռ�*/
    if(this_noterminal->first_number==0)
    {
        this_noterminal->FIRST=(char*)malloc(MAX_AMOUNT+1);
        memset(this_noterminal->FIRST,0,MAX_AMOUNT+1);
    }
    while(this_production&&this_production->source==this_noterminal->character)
    {
        TEMP2=this_production->result;
        while(*TEMP2)
        {
            if(is_appeared(*TEMP2,terminal_all))
            {
                temp[0]=this_production->result[0];
                combine(this_noterminal->FIRST,temp);
                break;
            }
            else
            {
                this_noterminal_temp=NOTERMINAL_HEAD.next;
                while(this_noterminal_temp->character!=*TEMP2)
                    this_noterminal_temp=this_noterminal_temp->next;
                if(this_noterminal_temp->first_number==0)/*û��first��*/
                    find_first(this_noterminal_temp,PRODUCTION_HEAD.next);
                combine(this_noterminal->FIRST,this_noterminal_temp->FIRST);
                help=this_noterminal->FIRST;
                while(*help&&*help!='^')
                    help++;
                if(*help&&*(TEMP2+1))/*����'^'*/
                {
                    while(*help)
                    {
                        *help=*(help+1);
                        help++;
                    }
                }
                else break;
                TEMP2++;
            }
        }
        this_production=this_production->next;
    }
    this_noterminal->first_number=strlen(this_noterminal->FIRST);
    return success;
}

/*��FOLLOW����*/
size_t find_follow(noterminal* this_noterminal,production *this_production)
{
    char* help=NULL;
    char* temp_result;
    int symbol=0;
    char terminal_array[2]= {0};
    noterminal* FOLLOW_TEMP,*FOLLOW_HELP;

    if(this_noterminal->follow_number==0)
    {
        this_noterminal->FOLLOW=(char* )malloc(MAX_AMOUNT+1);
        memset(this_noterminal->FOLLOW,0,MAX_AMOUNT+1);
    }

    /*��һ�����ս��������#*/
    if(this_noterminal==NOTERMINAL_HEAD.next)
        *this_noterminal->FOLLOW='#';

    while(this_production)
    {
        temp_result=this_production->result;
        /*һ������ʽδ��β*/
        while(*temp_result)
        {
            if(*temp_result!=this_noterminal->character)
            {
                temp_result++;
                continue;
            }
            temp_result++;
            if(!*temp_result)
                symbol=1;
            while(*temp_result)
            {
                if(is_appeared(*temp_result,terminal_all))
                {
                    terminal_array[0]=*temp_result;
                    combine(this_noterminal->FOLLOW,terminal_array);
                }
                else
                {
                    FOLLOW_TEMP=NOTERMINAL_HEAD.next;
                    while(FOLLOW_TEMP->character!=*temp_result)
                        FOLLOW_TEMP=FOLLOW_TEMP->next;
                    combine(this_noterminal->FOLLOW,FOLLOW_TEMP->FIRST);
                    help=this_noterminal->FOLLOW;
                    while(*help&&*help!='^')
                        help++;
                    if(*help)/*����'^'*/
                    {
                        while(*help)
                        {
                            *help=*(help+1);
                            help++;
                        }
                        symbol=1;
                    }
                    else
                    {
                        symbol=0;
                        break;
                    }
                }
                temp_result++;
            }
            if(symbol&&this_production->source!=this_noterminal->character)
            {
                FOLLOW_HELP=NOTERMINAL_HEAD.next;
                while(FOLLOW_HELP->character!=this_production->source)
                    FOLLOW_HELP=FOLLOW_HELP->next;
                if(FOLLOW_HELP->follow_number==0)
                    find_follow(FOLLOW_HELP,PRODUCTION_HEAD.next);
                combine(this_noterminal->FOLLOW,FOLLOW_HELP->FOLLOW);
                symbol=0;
            }
        }
        this_production=this_production->next;
    }
    this_noterminal->follow_number=strlen(this_noterminal->FOLLOW);
    return success;
}


/*�����˳�*/
void emergency(int model)
{
    current_noterminal=NOTERMINAL_HEAD.next;
    current_terminal=TERMINAL_HEAD.next;
    current_production=PRODUCTION_HEAD.next;
    while(current_noterminal)
    {
        NOTERMINAL_HEAD.next=current_noterminal->next;
        free(current_noterminal->FIRST);
        free(current_noterminal->FOLLOW);
        free(current_noterminal);
        current_noterminal=NOTERMINAL_HEAD.next;
    }
    while(current_terminal)
    {
        TERMINAL_HEAD.next=current_terminal->next;
        free(current_terminal);
        current_terminal=TERMINAL_HEAD.next;
    }
    while(current_production)
    {
        PRODUCTION_HEAD.next=current_production->next;
        free(current_production->result);
        free(current_production);
        current_production=PRODUCTION_HEAD.next;
    }
    printf("�˳��ɹ�\n");
    exit(0);
}

/*���뵽�ս��*/
void insert_to_terminal(char get)
{
    terminal *Temp_terminal=(terminal*)malloc(sizeof(terminal));
    if(!Temp_terminal)
    {
        perror("can`t malloc for this program\n");
        emergency(0);
    }
    Temp_terminal->character=get;
    Temp_terminal->next=NULL;
    current_terminal->next=Temp_terminal;
    current_terminal=Temp_terminal;/*������һ���ڵ�*/
    return ;
}

/*���뵽���ս��*/
void insert_to_noterminal(char get)
{
    noterminal *Temp_noterminal=(noterminal*)malloc(sizeof(noterminal));
    if(!Temp_noterminal)
    {
        perror("can`t malloc for this program\n");
        emergency(0);
    }
    Temp_noterminal->character=get;
    Temp_noterminal->next=NULL;
    Temp_noterminal->FIRST=NULL;
    Temp_noterminal->FOLLOW=NULL;
    Temp_noterminal->first_number=0;
    Temp_noterminal->follow_number=0;
    current_noterminal->next=Temp_noterminal;
    current_noterminal=Temp_noterminal;/*������һ���ڵ�*/
    return ;
}

/*���뵽����ʽ*/
void insert_to_production(char source , char* result)
{

    char TEMP[20]= {0};
    int COUNT=0,number=0,length=0,exit_condition=strlen(result);
    production *Temp_production;
    for (COUNT=0; COUNT!=exit_condition+1; COUNT++)
    {
        if(*result=='-'&&*(result+1)=='>')
        {
            result+=2;
            COUNT+=2;
        }
        if((*result!='|')&&(*result))
            TEMP[number++]=*result;
        else
        {
            Temp_production=(production*)malloc(sizeof(production));
            length=strlen(TEMP)+1;
            Temp_production->result=(char* )malloc(length);
            memset(Temp_production->result,0,length);
            strncpy(Temp_production->result,TEMP,length-1);
            memset(TEMP,0,sizeof(TEMP));
            Temp_production->source=source;
            Temp_production->next=NULL;
            current_production->next=Temp_production;
            current_production=Temp_production;
            number=0;
        }
        result++;
    }
    return ;
}

/*������ݹ�*/
void eliminate_left_recursion(void)
{
    int number=0;
    char new_char[3]= {0},TEMP_RESULT[20],temp_empty[3]= {'^',0,0};
    production  *Temp_production=PRODUCTION_HEAD.next;
    production  *Temp_FREE;
    terminal *temp=TERMINAL_HEAD.next;
    while(Temp_production)
    {
        if(Temp_production->source==Temp_production->result[0])
        {
            memset(TEMP_RESULT,0,sizeof(TEMP_RESULT));
            new_char[0]=Temp_production->source-'A'+'a';
            /*���Ƶ��µĲ���ʽ*/
            strcat(TEMP_RESULT,Temp_production->result+1);
            strcat(TEMP_RESULT,new_char);
            insert_to_noterminal(new_char[0]);
            insert_to_production(new_char[0],TEMP_RESULT);
            insert_to_production(new_char[0],temp_empty);

            /*�޸ĵ�ǰ�Ĳ���ʽ*/
            memset(TEMP_RESULT,0,sizeof(TEMP_RESULT));
            strcat(TEMP_RESULT,Temp_production->next->result);
            strcat(TEMP_RESULT,new_char);
            memset(Temp_production->result,0,strlen(Temp_production->result));
            strncpy(Temp_production->result,TEMP_RESULT,strlen(TEMP_RESULT));

            Temp_FREE= Temp_production->next;
            Temp_production->next=Temp_production->next->next;
            free(Temp_FREE);
            continue;
        }
        Temp_production=Temp_production->next;
    }
    while(temp)
    {
        terminal_all[number++]=temp->character;
        temp=temp->next;
    }
    return ;
}

void Test_read(void)
{
    int number=1;
    production *TEMP_PRODUCTION=PRODUCTION_HEAD.next;
    terminal *TEMP_TERMINAL=TERMINAL_HEAD.next;
    noterminal *TEMP_NOTERMINAL=NOTERMINAL_HEAD.next;

    printf("\n����ʽ\n");
    for(number=1; TEMP_PRODUCTION!=NULL; TEMP_PRODUCTION=TEMP_PRODUCTION->next,number++)
    {
        printf("%d\t%c\t%s\n",number,TEMP_PRODUCTION->source,TEMP_PRODUCTION->result);
    }
    printf("\n�ս��\n");
    for(; TEMP_TERMINAL!=NULL; TEMP_TERMINAL=TEMP_TERMINAL->next)
    {
        printf("%c\t",TEMP_TERMINAL->character);
    }
    printf("\n");
    printf("\n���ս��\n");
    for(; TEMP_NOTERMINAL!=NULL; TEMP_NOTERMINAL=TEMP_NOTERMINAL->next)
    {
        printf("%c\t",TEMP_NOTERMINAL->character);
    }
    printf("\n��ȡ����\n%s\n%s\n",TEST_LIST[0],TEST_LIST[1]);
    printf("\n");
    return ;
}

size_t is_appeared(char tobejudged,char*source)
{
    size_t length=strlen(source),counts=0;
    while((counts!=length)&&(*source!=tobejudged))
    {
        counts++;
        source++;
    }
    return counts==length?!success: success;
}

void combine(char* destinction,char* source)
{
    char temp[2]= {0};
    while(*source)
    {
        if(!is_appeared(*source,destinction))
        {
            temp[0]=*source;
            strcat(destinction,temp);
        }
        source++;
    }
    return ;
}


void prediction(void)
{
    noterminal* TEMP_NOTERMINAL=NOTERMINAL_HEAD.next;
    while(TEMP_NOTERMINAL!=NULL)
    {
        find_first(TEMP_NOTERMINAL,PRODUCTION_HEAD.next);
        TEMP_NOTERMINAL=TEMP_NOTERMINAL->next;
    }
    test();

    TEMP_NOTERMINAL=NOTERMINAL_HEAD.next;
    while(TEMP_NOTERMINAL!=NULL)
    {
        find_follow(TEMP_NOTERMINAL,PRODUCTION_HEAD.next);
        TEMP_NOTERMINAL=TEMP_NOTERMINAL->next;
    }
    return ;
}

void test_follow(void)
{
    noterminal *TEMP_NOTERMINAL=NOTERMINAL_HEAD.next;

    for(; TEMP_NOTERMINAL!=NULL; TEMP_NOTERMINAL=TEMP_NOTERMINAL->next)
    {
        printf("%c\tfollow number=%d\tfollow=%s\n",
               TEMP_NOTERMINAL->character,
               TEMP_NOTERMINAL->follow_number,
               TEMP_NOTERMINAL->FOLLOW);
    }
    printf("\n");
    return ;
}

void prediction_table(void)
{
    int line=0,row=0,current_character=0,number=0;
    char* FIRST_CLUM,*test_exper;
    noterminal* temp_noterminal=NOTERMINAL_HEAD.next,*temp_noterminal21;
    terminal*   temp_terminal=TERMINAL_HEAD.next;
    production* temp_production=PRODUCTION_HEAD.next;
    char hah[5][7];
    memset(hah,0,sizeof(hah));
    for(line=0; line<5; line++)
    {
        for(row=0; row<7; row++)
            hah[line][row]=0;
    }
    line=0;
    while(temp_production)
    {
        row=0;
        if(is_appeared(*temp_production->result,terminal_all)&&(*temp_production->result!='^'))
        {
            while(terminal_all[row]!=*temp_production->result)
                row++;
            hah[current_character][row]=line+1;
        }
        else
        {
            temp_noterminal=NOTERMINAL_HEAD.next;
            if(*temp_production->result=='^')
            {
                while(temp_noterminal->character!=temp_production->source)
                    temp_noterminal=temp_noterminal->next;
                FIRST_CLUM=temp_noterminal->FOLLOW;

                if(is_appeared('#',FIRST_CLUM))
                {
                    row=0;
                    while(terminal_all[row] != '#')
                        row++;
                    hah[current_character][row]=line+1;
                }
                while(*FIRST_CLUM)
                {
                    row=0;
                    while(terminal_all[row]!=*FIRST_CLUM)
                        row++;
                    hah[current_character][row]=line+1;
                    FIRST_CLUM++;
                }
                if(temp_production->next&&temp_production->source!=temp_production->next->source)
                    current_character++;
                temp_production=temp_production->next;
                line++;
                continue;
            }
            /*�Ƿ��ս��*/
            while(temp_noterminal->character!=*temp_production->result)
                temp_noterminal=temp_noterminal->next;
            FIRST_CLUM=temp_noterminal->FIRST;
            while(*FIRST_CLUM)
            {
                row=0;
                while(terminal_all[row]!=*FIRST_CLUM)
                    row++;
                hah[current_character][row]=line+1;
                FIRST_CLUM++;
            }
            temp_noterminal21=NOTERMINAL_HEAD.next;
            while(temp_noterminal21->character!=temp_production->source)
                temp_noterminal21=temp_noterminal21->next;
            if(is_appeared('^',temp_noterminal->FIRST)&&is_appeared('#',temp_noterminal21->FOLLOW))
            {
                row=0;
                while(terminal_all[row]!=*FIRST_CLUM)
                    row++;
                hah[line][row]=line+1;
                FIRST_CLUM++;
            }
        }
        if(temp_production->next&&temp_production->source!=temp_production->next->source)
            current_character++;
        temp_production=temp_production->next;
        line++;
    }

    printf("\nԤ�������\n\n");
    printf(" \t");
    for(temp_terminal=TERMINAL_HEAD.next; temp_terminal; temp_terminal=temp_terminal->next)
        printf("%c\t",temp_terminal->character);
    temp_noterminal=NOTERMINAL_HEAD.next;
    for(line=0; line<5; line++)
    {
        printf("\n%c\t",temp_noterminal->character);
        for(row=0; row<7; row++)
            printf("%c\t",hah[line][row]==0?' ':(hah[line][row]-0+'0'));
        temp_noterminal=temp_noterminal->next;
    }
    printf("\n\n");
    system("pause");
    printf("\n\n");
    memset(TEST_STACK,0,sizeof(TEST_STACK));
    init_stack();
    test_exper=TEST_LIST[0];
    test_exper[strlen(test_exper)]='#';
    STACK_PUSH(NOTERMINAL_HEAD.next->character);
    while(!STACK_EMPTY())
    {
        printf("����ջ\t");
        for(number=0; number<=amount; number++)
            printf("%c",TEST_STACK[number]);
        printf("\tʣ���ַ���\t%s\n",test_exper);
        if(TEST_STACK[amount]==*test_exper)
        {
            STACK_POP();
            test_exper++;
        }
        else
        {
            line=0;
            row=0;
            temp_noterminal=NOTERMINAL_HEAD.next;
            while(temp_noterminal->character!=TEST_STACK[amount])
            {
                temp_noterminal=temp_noterminal->next;
                line++;
            }
            while(terminal_all[row]!=*test_exper)
                row++;
            row=hah[line][row];
            if(!row)
                break;
            temp_production=PRODUCTION_HEAD.next;
            while(--row)
                temp_production=temp_production->next;

            FIRST_CLUM=temp_production->result;
            current_character=strlen(FIRST_CLUM);
            FIRST_CLUM=FIRST_CLUM+current_character-1;
            STACK_POP();
            while(current_character)
            {
                if(*FIRST_CLUM!='^')
                    STACK_PUSH(*FIRST_CLUM);
                FIRST_CLUM--;
                current_character--;
            }
        }
    }

    printf("����ջ\t");
    for(number=0; number<=amount; number++)
        printf("%c",TEST_STACK[number]);
    printf("\tʣ���ַ���\t%s\n",test_exper);
    if(STACK_EMPTY()&&*test_exper=='#')
        printf("\n�Ϸ�����\n");
    else
        printf("\n���Ϸ�����\n");
    return ;
}


void STACK_POP(void)
{
    if(STACK_EMPTY())
    {
        printf("ջ��\n");
        emergency(2);
    }
    amount--;
    return ;
}
size_t STACK_EMPTY()
{
    return amount==0? success:!success;
}
size_t STACK_FULL()
{
    return amount==19? success:!success;
}
void STACK_PUSH(char source)
{
    if(STACK_FULL())
    {
        printf("ջ��\n");
        emergency(2);
    }
    TEST_STACK[++amount]=source;
    return ;
}

void init_stack(void)
{
    amount=0;
    TEST_STACK[amount]='#';
    return ;
}
