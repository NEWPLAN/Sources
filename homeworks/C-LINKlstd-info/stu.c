#include "stu.h"
#include<stdio.h>
#include<stdlib.h>
#include <conio.h>
#include <string.h>

//void bound (char x,int n) //显示分隔符    我写的bound
//	{
//	int i;
//	if(i=0,i<n,i++)
//	printf("x");
//}

//八百多行代码之前一直崩溃的原因是  addstu scanf chinese math english 没加&

void bound (char ch,int n)
{
    while(n--)
        putch(ch);

    printf("\n");
    return;
}



void checkfirst(char password[])  //检查初始化状态
{

    FILE *fp,*fp1;
    char pwd[9],pwd1[9],ch;
    int i;

    if((fp=fopen("config.dat","rb"))==NULL)
    {
        printf("\n亲，新系统还没有初始化哦~单击任意键进行初始化~\n");
        bound('_',50);
        getch();
        do
        {
            printf("\n设置登录密码，最多8位字母哟~");
            for (i=0; i<8&&((pwd[i]=getch())!=13); i++)
                putch('*');

            printf("\n再次确认密码哟~");
            for (i=0; i<8&&((pwd1[i]=getch())!=13); i++)
                putch('*');

            pwd[i]='\0';
            pwd1[i]='\0';

            if (strcmp(pwd,pwd1)!=0)
                printf("\n两次输入的密码不同哟，请重新输入~\n\n");
            else break;
        }
        while(1);

        if((fp1=fopen("config.dat","wb"))==NULL)
        {
            printf("创立系统配置文件失败咯~按任意键退出系统");
            getch();
            exit(1) ;
        }

        i=0;
        while(pwd[i])
        {
            putw(pwd[i],fp1);
            i++;
        }

        fclose(fp1);

        printf("\n\n恭喜个哟~系统配置成功，按任意键退出系统并重新进入哈~\n");
        getch();
        exit(1);
    }
    else
    {
        i=0;
        while(!feof(fp)&&i<8)
            pwd[i++]=getw(fp);

        pwd[i]='\0';

        if(i>=8)i--;
        while(pwd[i]!=-1&&i>=0)
            i--;

        pwd[i]='\0';
        strcpy(password,pwd);


    }
}




void login(char password[])//检查密码
{
    int i,n=3;
    char pwd[9];

    do
    {
        printf("报上你的口令兄弟：");
        for(i=0; i<8&&((pwd[i]=getch())!=13); i++)
            putch('*');

        pwd[i]='\0';
        if (strcmp(pwd,password))
        {
            printf("\n大兄弟密码错了,再来一次！\n");
            system("cls");              //清屏
            n--;
        }
        else break;
    }
    while(n>0);

    if(!n)
    {
        printf("你被认为是非法用户，请关机~\n");
        getch();
        exit(1);
    }
}

void readdata(char* gfirst,STU **stu_first,STU **stu_end)   //从文件中获取员工信息，创建链表
{
    FILE *fp;
    STU *stu1;

    if((fp=fopen("stu.dat","rb"))==NULL)       //读方式打开文件
    {
        (*gfirst)=1;
        return;
    }


    while(!feof(fp))
    {

        stu1=(STU *)malloc(sizeof(STU));
        if (stu1==NULL)
        {
            printf("内存分配失败咯\n");
            getch();
            return;
        }                                 //字符串数组直接用名字不用&
        /*modified by NEWPLAN*/
        if(!fread(stu1,sizeof(STU),1,fp))
        {
            free(stu1);
            continue;
        }
        stu1->next=NULL;
        if(*stu_first==NULL)
        {
            *stu_first=stu1;
            *stu_end=stu1;
        }
        else
        {
            (*stu_end)->next=stu1;
            *stu_end=stu1;
        }
        (*stu_end)->next=NULL;

    }
    (*gfirst)=0;
    fclose(fp);
}

void menu(STU *stu_first,char *gfirst,char *gsave,STU *stu_end,char password[])    //显示主菜单
{
    char choice;

    //system("cls");

    do
    {
        printf("\t学生管理系统\n");
        bound('_',30) ;
        printf("\t1.输入学生信息\n");
        printf("\t2.查询学生信息\n");
        printf("\t3.显示学生信息\n");
        printf("\t4.修改学生信息\n");
        printf("\t5.删除学生信息\n");
        printf("\t6.统计学生信息\n");
        printf("\t7.重设系统密码\n");
        printf("\t0.退出系统\n");
        bound('_',30);
        printf("\n请选择菜单：");

        do
        {
            fflush(stdin);
            choice=getchar();
            //system("cls");

            switch(choice)
            {
            case'1':                         //输入
                addstu(stu_first,stu_end,gfirst);
                break;

            case'2':                         //查询
                if(*gfirst)
                {
                    printf("系统还没有学生信息哟，请先添加信息");
                    getch();
                    break;
                }
                findstu(stu_first);
                break;

            case'3':                   //显示
                if(*gfirst)
                {
                    printf("系统还没有学生信息哟，请先添加信息");
                    getch();
                    break;
                }
                liststu(stu_first);
                break;

            case'4':                         //修改
                if(*gfirst)
                {
                    printf("系统还没有学生信息哟，请先添加信息");
                    getch();
                    break;
                }
                modifystu(stu_first, gsave);
                break;

            case'5':                        //删除
                if(*gfirst)
                {
                    printf("系统还没有学生信息哟，请先添加信息");
                    getch();
                    break;
                }
                delstu(stu_first, gsave);
                break;

            case'6':                        //统计
                if(*gfirst)
                {
                    printf("系统还没有学生信息哟，请先添加信息");
                    getch();
                    break;
                }
                summarystu(stu_first);
                break;

            case'7':                        //重设密码
                resetpwd(password);
                break;

            case'0':                        //退出
                savedata(gsave,stu_first);
                exit(0);
            }
        }
        while(choice<'0'||choice>'7');

        //system("cls");
    }
    while(1);
}

void savedata(char *gsave,STU *stu_first)									//将链表并保存到文件
{
    FILE *fp;
    STU *stu1;

    if((*gsave)==0)
        return;

    if((fp=fopen("stu.dat","wb"))==NULL)
    {
        printf("打开文件stu.dat出错了哟~\n");
        getch();
        return;
    }

    stu1=stu_first;
    while(stu1)
    {
        fwrite(stu1,sizeof(STU),1,fp);
        stu1=stu1->next;
    }

    (*gsave)=0;
    fclose(fp);
}

void addstu(STU *stu_first,STU *stu_end,char* gfirst)           			//输入学生信息
{
    FILE *fp;
    STU *stu1;
    int i=0;
    char choice='Y';
    if((fp=fopen("stu.dat","ab"))==NULL)
    {
        printf("打开文件stu.dat出错了哟~\n");
        getch();
        return;
    }

    do
    {
        i++;
        stu1=(STU *)malloc(sizeof(STU));

        if (stu1==NULL)
        {
            printf("内存分配失败咯，按任意键返回");
            getch();
            return;
        }

        printf("输入第%d个学生的信息\n",i);
        bound('_',20);

        printf("学号：");
        scanf("%s",stu1->num);

        printf("专业：");
        scanf("%s",stu1->pro);

        printf("名字:");
        scanf("%s",stu1->name);
        getchar();
        printf("性别（F/M)");
        scanf("%c",&stu1->sex);


        printf("语文成绩");
        scanf("%d",&stu1->chinese);

        printf("数学成绩");
        scanf("%d",&stu1->math);

        printf("英语成绩");
        scanf("%d",&stu1->english);

        printf("电话");
        scanf("%s",stu1->tel);

        printf("Q号");
        scanf("%s",stu1->qq);

        printf("地址");
        scanf("%s",stu1->address);

        stu1->next=NULL;
        if  (stu_first==NULL)
        {
            stu_first=stu1;
            stu_end=stu1;
        }
        else
        {
            stu_end->next=stu1;
            stu_end=stu1;
        }

        fwrite(stu_end,sizeof(STU),1,fp);

        (*gfirst)=0;
        printf("\n");
        bound('_',20);
        printf("\n还要继续嘛~(y/n)");
        fflush(stdin);
        choice=getchar();

        if(toupper(choice)!='Y')
        {
            fclose(fp);
            printf("\n输入完成啦。任意键返回~");
            getch();
            return;

        }
        system("cls");

    }
    while (1);

}


void findstu(STU *stu_first)    //查询同学信息
{
    int choice,ret=0,num;
    char str[13];
    STU *stu1;

    system("cls");

    do
    {
        printf("\t查询学生信息\n");
        bound('_',25);
        printf("\t1.按姓名查询\n");
        printf("\t2.按学号查询\n");
        printf("\t3.按电话查询\n");
        printf("\t4.按QQ查询\n");
        printf("\t0.返回主菜单\n");
        bound('_',25);
        printf("\n请选择菜单哟：");

        do
        {
            fflush(stdin);
            choice=getchar();
            system("cls");

            switch(choice)
            {
            case'1':
                printf("输入要查询的同学的姓名；");
                scanf("%s",str);

                stu1=findname(str,stu_first);
                displaystu(stu1,"姓名",str);
                getch();
                break;

            case'2':
                printf("输入要查询的同学的学号；");
                scanf("%s",str);

                stu1=findnum(str,stu_first);
                displaystu(stu1,"学号",str);
                getch();
                break;

            case'3':
                printf("输入要查询的同学的电话；");
                scanf("%s",str);

                stu1=findtelephone(str,stu_first);
                displaystu(stu1,"电话",str);
                getch();
                break;

            case'4':
                printf("输入要查询的同学的Q号；");
                scanf("%s",str);

                stu1=findqq(str,stu_first);
                displaystu(stu1,"Q号",str);
                getch();
                break;

            case'0':
                ret=1;
                break;
            }
        }
        while(choice<0||choice>'4');

        system("cls");
        if(ret) break;
    }
    while(1);
}

STU  *findname(char *name,STU *stu_first)         //按姓名在链表中查询
{
    STU *stu1;
    stu1=stu_first;

    while(stu1)
    {
        if(strcmp(name,stu1->name)==0)
            return stu1;
        stu1=stu1->next;
    }
    return NULL;
}

STU *findnum(char *name,STU *stu_first)       //按学号查询
{
    STU *stu1;

    stu1=stu_first;
    while(stu1)
    {
        if(strcmp(name,stu1->num)==0)
            return stu1;
        stu1=stu1->next;
    }

    return NULL;
}

STU *findtelephone(char *name,STU *stu_first)     //按电话查询
{
    STU *stu1;

    stu1=stu_first;
    while(stu1)
    {
        if(strcmp(name,stu1->tel)==0)
            stu1=stu1->next;
    }
    return NULL;
}

STU *findqq(char *name,STU *stu_first)         //按q号查询
{
    STU *stu1;

    stu1=stu_first;
    while(stu1)
    {
        if(strcmp(name,stu1->qq)==0)
            return stu1;
        stu1=stu1->next;
    }
    return NULL;
}

void displaystu(STU *stu,char *field,char *name)
{
    if(stu)
    {
        printf("\n%s:%s信息如下；\n",field,name);
        bound('_',20);
        printf("学号:%s\n",stu->num);
        printf("专业:%s\n",stu->pro);
        printf("名字:%s\n",stu->name);
        printf("性别:%c\n",stu->sex);
        printf("语文:%d\n",stu->chinese);
        printf("数学:%d\n",stu->math);
        printf("英语:%d\n",stu->english);
        printf("电话:%s\n",stu->tel);
        printf("Q号:%s\n",stu->qq);
        printf("地址:%s\n",stu->address);
        bound('_',20);
    }
    else
    {
        bound('_',20);
        printf("\n系统中没有%s为：%s的同学的信息哇~\n",field,name);
    }
    return;
}

void liststu(STU *stu_first)    //显示同学信息
{
    STU *stu1;
    printf("\n同学列表：\n");
    bound('_',80);
    stu1=stu_first;

    while(stu1)
    {
        printf("学号:%s",stu1->num);
        printf("专业:%s",stu1->pro);
        printf("名字:%s",stu1->name);
        printf("性别:%c",stu1->sex);
        printf("语文:%d",stu1->chinese);
        printf("数学:%d",stu1->math);
        printf("英语:%d",stu1->english);
        printf("电话:%s",stu1->tel);
        printf("Q号:%s",stu1->qq);
        printf("地址:%s\n",stu1->address);
        bound('_',80);
        stu1=stu1->next;
    }

    printf("\n按任意键返回~\n");
    getch();
    return;
}

void modifystu(STU *stu_first, char *gsave)   //修改同学信息
{
    STU *stu1;
    char name[10] ,*str;
    int choice;

    printf("\n输入要修改的同学的姓名：");
    scanf("%s",name);

    stu1=findname(name,stu_first);
    displaystu(stu1,"姓名",name);

    if(stu1)
    {
        printf("\n选择要修改的项目哈~\n");
        bound('_',35);
        printf("1.修改语文成绩    2.修改数学成绩\n");
        printf("3.修改英语成绩    4.修改电话\n");
        printf("5.修改q号         6.修改地址\n");
        printf("0.返回~\n");
        bound('_',35);
        printf("\n请选择：");
        do
        {

            fflush(stdin);
            choice=getchar();
            switch(choice)
            {
            case'1':
                stu1->chinese=modi_grade(stu1->chinese);
                break;

            case'2':
                stu1->math=modi_grade(stu1->math);
                break;

            case'3':
                stu1->english=modi_grade(stu1->english);
                break;

            case'4':
                str=modi_field("电话",stu1->tel,12);
                if(str!=NULL)
                {
                    strcpy(stu1->tel,str);
                    free(str);
                }

            case'5':
                str=modi_field("Q号",stu1->qq,10);
                if(str!=NULL)
                {
                    strcpy(stu1->qq,str);
                    free(str);
                }

            case'6':
                str=modi_field("地址",stu1->address,30);
                if(str!=NULL)
                {
                    strcpy(stu1->address,str);
                    free(str);
                }

            case'0':
                return;
            }

        }
        while(choice<0||choice>6);

        (*gsave)=1;
        savedata(gsave, stu_first);
        printf("修改完成~按任意键返回\n");
        getch();
    }
    return;
}

int modi_grade(int n)
{
    int grade;

    printf("原来的成绩：%d\n",n);
    printf("新的的成绩：");
    scanf("%d",&grade);

    return grade;
}


char *modi_field(char *field,char *s,int n)
{
    char *str;
    str=malloc(sizeof(char)*n);
    if(str==NULL);
    {
        printf("内存分配失败咯~按任意键返回哈~\n");
        getch();
        return NULL;
    }

    printf("原%s:%s\n",field,s);
    printf("修改为（不超过%d个字符)",n );
    scanf("%s",str);

    return str;
}

void delstu(STU *stu_first, char *gsave)       //删除同学信息
{
    int i=0,find=0;
    STU *stu1,*stu2;
    char name[10],choice;

    system("cls");
    printf("\n输入要删除的同学姓名：");
    scanf("%s",name);

    stu1=stu_first;
    stu2=stu1;
    while(stu1)
    {
        if(strcmp(stu1->name,name)==0)
        {
            find=1;
            system("cls");

            printf("学生：%s信息如下哈~：\n",stu1->name);
            bound('_',20);
            printf("学号:%s",stu1->num);
            printf("专业:%s",stu1->pro);
            printf("名字:%s",stu1->name);
            printf("性别:%c",stu1->sex);
            printf("语文:%d",stu1->chinese);
            printf("数学:%d",stu1->math);
            printf("英语:%d",stu1->english);
            printf("电话:%s",stu1->tel);
            printf("Q号:%s",stu1->qq);
            printf("地址:%s",stu1->address);
            bound('_',20);
            printf("真的要删除该同学的信息吗~（y/n)");

            fflush(stdin);
            choice=getchar();

            if(choice!='y'&&choice!='Y')
                return;
            if(stu1==stu_first)
                stu_first=stu1->next;
            else stu2->next=stu1->next;

            free(stu1);
            (*gsave)=1;
            savedata(gsave, stu_first);      //保存数据
            return;
        }
        else
        {
            stu2=stu1;
            stu1=stu1->next;
        }
    }

    if(!find)
    {
        bound('_',30);
        printf("啊~没找到名为%s的同学啊~TA的信息是不是被黑的次掉了咩~\n",name);
        getch();
    }
    return;
}

void resetpwd(char password[])          //重设密码
{
    char pwd[9],pwd1[9],ch;
    int i;
    FILE *fp1;

    system("cls");

    printf("\n输入旧密码哈~");
    for(i=0; i<8&&((pwd[i]=getch())!=13); i++)
        putch('*');

    pwd[i]='\0';
    if(strcmp(password,pwd)!=0)
    {
        printf("密码错了大兄弟！按任意键回去吧~\n");
        getch();
        return;
    }
    do
    {
        printf("\n输入新密码哟（最多8位~）：");
        for(i=0; i<8&&((pwd[i]=getch())!=13); i++)
            putch('*');

        printf("\n确认下密码哈~");
        for(i=0; i<8&&((pwd1[i]=getch())!=13); i++)
            putch('*');

        pwd[i]='\0';
        pwd1[i]='\0';
        if ((strcmp(pwd,pwd1))!=0)
            printf("\n两次输入密码不同啊大兄弟！，重新输入哈~\n") ;
        else break;
    }
    while(1);

    if((fp1=fopen("config.dat","wb"))==NULL)
    {
        printf("\n创建系统配置文件失败咯~按任意键回去吧小伙子\n");
        getch();
        exit(1);
    }

    i=0;
    while(pwd[i])
    {
        putw(pwd[i],fp1);
        i++;
    }
    fclose(fp1);
    printf("\n修改密码成功了哟~按任意键返回~\n");
    getch();
    return;
}
void summarystu(STU* stu_first)      //数据统计
{
    STU *stu1;
    int pass_sum_chi=0,ecl_sum_chi=0,pass_sum_mat=0,ecl_sum_mat=0;
    int pass_sum_eng=0,ecl_sum_eng=0,man=0,woman=0;
    int man_pass_sum_chi=0,man_ecl_sum_chi=0;
    int man_pass_sum_mat=0,man_ecl_sum_mat=0;
    int man_pass_sum_eng=0,man_ecl_sum_eng=0;
    int woman_pass_sum_chi=0,woman_ecl_sum_chi=0;
    int woman_pass_sum_mat=0,woman_ecl_sum_mat=0;
    int woman_pass_sum_eng=0,woman_ecl_sum_eng=0;


    stu1=stu_first;

    while(stu1)
    {

        if(stu1->chinese>=60)
            pass_sum_chi++;

        if(stu1->chinese>=90)
            ecl_sum_chi++;

        if(stu1->math>=60)
            pass_sum_mat++;

        if(stu1->math>=90)
            ecl_sum_mat++;

        if(stu1->english>=60)
            pass_sum_eng++;

        if(stu1->english>=60)
            ecl_sum_eng++;

        if(stu1->sex=='M'||stu1->sex=='m')  //注意1.字符可直接比较 2.字符要加'' 3.用==
        {
            man++;
            if(stu1->chinese>=60)
                man_pass_sum_chi++;

            if(stu1->chinese>=90)
                man_ecl_sum_chi++;

            if(stu1->math>=60)
                man_pass_sum_mat++;

            if(stu1->math>=90)
                man_ecl_sum_mat++;

            if(stu1->english>=60)
                man_pass_sum_eng++;

            if(stu1->english>=60)
                man_ecl_sum_eng++;

        }
        else
        {
            woman++;
            if(stu1->chinese>=60)
                woman_pass_sum_chi++;

            if(stu1->chinese>=90)
                woman_ecl_sum_chi++;

            if(stu1->math>=60)
                woman_pass_sum_mat++;

            if(stu1->math>=90)
                woman_ecl_sum_mat++;

            if(stu1->english>=60)
                woman_pass_sum_eng++;

            if(stu1->english>=60)
                man_ecl_sum_eng++;
        }
        stu1=stu1->next;
    }

    printf("语文及格人数%d",pass_sum_chi);
    printf("语文优秀人数%d",ecl_sum_mat);
    printf("数学优秀人数%d",pass_sum_mat);
    printf("数学优秀人数%d",ecl_sum_mat);
    printf("英语优秀人数%d",pass_sum_eng);
    printf("英语优秀人数%d",ecl_sum_eng);

    printf("男生人数%d",man);
    printf("女生%d",woman);
    return;
}





