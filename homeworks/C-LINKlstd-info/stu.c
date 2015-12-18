#include "stu.h"
#include<stdio.h>
#include<stdlib.h>
#include <conio.h>
#include <string.h>

//void bound (char x,int n) //��ʾ�ָ���    ��д��bound
//	{
//	int i;
//	if(i=0,i<n,i++)
//	printf("x");
//}

//�˰ٶ��д���֮ǰһֱ������ԭ����  addstu scanf chinese math english û��&

void bound (char ch,int n)
{
    while(n--)
        putch(ch);

    printf("\n");
    return;
}



void checkfirst(char password[])  //����ʼ��״̬
{

    FILE *fp,*fp1;
    char pwd[9],pwd1[9],ch;
    int i;

    if((fp=fopen("config.dat","rb"))==NULL)
    {
        printf("\n�ף���ϵͳ��û�г�ʼ��Ŷ~������������г�ʼ��~\n");
        bound('_',50);
        getch();
        do
        {
            printf("\n���õ�¼���룬���8λ��ĸӴ~");
            for (i=0; i<8&&((pwd[i]=getch())!=13); i++)
                putch('*');

            printf("\n�ٴ�ȷ������Ӵ~");
            for (i=0; i<8&&((pwd1[i]=getch())!=13); i++)
                putch('*');

            pwd[i]='\0';
            pwd1[i]='\0';

            if (strcmp(pwd,pwd1)!=0)
                printf("\n������������벻ͬӴ������������~\n\n");
            else break;
        }
        while(1);

        if((fp1=fopen("config.dat","wb"))==NULL)
        {
            printf("����ϵͳ�����ļ�ʧ�ܿ�~��������˳�ϵͳ");
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

        printf("\n\n��ϲ��Ӵ~ϵͳ���óɹ�����������˳�ϵͳ�����½����~\n");
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




void login(char password[])//�������
{
    int i,n=3;
    char pwd[9];

    do
    {
        printf("������Ŀ����ֵܣ�");
        for(i=0; i<8&&((pwd[i]=getch())!=13); i++)
            putch('*');

        pwd[i]='\0';
        if (strcmp(pwd,password))
        {
            printf("\n���ֵ��������,����һ�Σ�\n");
            system("cls");              //����
            n--;
        }
        else break;
    }
    while(n>0);

    if(!n)
    {
        printf("�㱻��Ϊ�ǷǷ��û�����ػ�~\n");
        getch();
        exit(1);
    }
}

void readdata(char* gfirst,STU **stu_first,STU **stu_end)   //���ļ��л�ȡԱ����Ϣ����������
{
    FILE *fp;
    STU *stu1;

    if((fp=fopen("stu.dat","rb"))==NULL)       //����ʽ���ļ�
    {
        (*gfirst)=1;
        return;
    }


    while(!feof(fp))
    {

        stu1=(STU *)malloc(sizeof(STU));
        if (stu1==NULL)
        {
            printf("�ڴ����ʧ�ܿ�\n");
            getch();
            return;
        }                                 //�ַ�������ֱ�������ֲ���&
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

void menu(STU *stu_first,char *gfirst,char *gsave,STU *stu_end,char password[])    //��ʾ���˵�
{
    char choice;

    //system("cls");

    do
    {
        printf("\tѧ������ϵͳ\n");
        bound('_',30) ;
        printf("\t1.����ѧ����Ϣ\n");
        printf("\t2.��ѯѧ����Ϣ\n");
        printf("\t3.��ʾѧ����Ϣ\n");
        printf("\t4.�޸�ѧ����Ϣ\n");
        printf("\t5.ɾ��ѧ����Ϣ\n");
        printf("\t6.ͳ��ѧ����Ϣ\n");
        printf("\t7.����ϵͳ����\n");
        printf("\t0.�˳�ϵͳ\n");
        bound('_',30);
        printf("\n��ѡ��˵���");

        do
        {
            fflush(stdin);
            choice=getchar();
            //system("cls");

            switch(choice)
            {
            case'1':                         //����
                addstu(stu_first,stu_end,gfirst);
                break;

            case'2':                         //��ѯ
                if(*gfirst)
                {
                    printf("ϵͳ��û��ѧ����ϢӴ�����������Ϣ");
                    getch();
                    break;
                }
                findstu(stu_first);
                break;

            case'3':                   //��ʾ
                if(*gfirst)
                {
                    printf("ϵͳ��û��ѧ����ϢӴ�����������Ϣ");
                    getch();
                    break;
                }
                liststu(stu_first);
                break;

            case'4':                         //�޸�
                if(*gfirst)
                {
                    printf("ϵͳ��û��ѧ����ϢӴ�����������Ϣ");
                    getch();
                    break;
                }
                modifystu(stu_first, gsave);
                break;

            case'5':                        //ɾ��
                if(*gfirst)
                {
                    printf("ϵͳ��û��ѧ����ϢӴ�����������Ϣ");
                    getch();
                    break;
                }
                delstu(stu_first, gsave);
                break;

            case'6':                        //ͳ��
                if(*gfirst)
                {
                    printf("ϵͳ��û��ѧ����ϢӴ�����������Ϣ");
                    getch();
                    break;
                }
                summarystu(stu_first);
                break;

            case'7':                        //��������
                resetpwd(password);
                break;

            case'0':                        //�˳�
                savedata(gsave,stu_first);
                exit(0);
            }
        }
        while(choice<'0'||choice>'7');

        //system("cls");
    }
    while(1);
}

void savedata(char *gsave,STU *stu_first)									//���������浽�ļ�
{
    FILE *fp;
    STU *stu1;

    if((*gsave)==0)
        return;

    if((fp=fopen("stu.dat","wb"))==NULL)
    {
        printf("���ļ�stu.dat������Ӵ~\n");
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

void addstu(STU *stu_first,STU *stu_end,char* gfirst)           			//����ѧ����Ϣ
{
    FILE *fp;
    STU *stu1;
    int i=0;
    char choice='Y';
    if((fp=fopen("stu.dat","ab"))==NULL)
    {
        printf("���ļ�stu.dat������Ӵ~\n");
        getch();
        return;
    }

    do
    {
        i++;
        stu1=(STU *)malloc(sizeof(STU));

        if (stu1==NULL)
        {
            printf("�ڴ����ʧ�ܿ��������������");
            getch();
            return;
        }

        printf("�����%d��ѧ������Ϣ\n",i);
        bound('_',20);

        printf("ѧ�ţ�");
        scanf("%s",stu1->num);

        printf("רҵ��");
        scanf("%s",stu1->pro);

        printf("����:");
        scanf("%s",stu1->name);
        getchar();
        printf("�Ա�F/M)");
        scanf("%c",&stu1->sex);


        printf("���ĳɼ�");
        scanf("%d",&stu1->chinese);

        printf("��ѧ�ɼ�");
        scanf("%d",&stu1->math);

        printf("Ӣ��ɼ�");
        scanf("%d",&stu1->english);

        printf("�绰");
        scanf("%s",stu1->tel);

        printf("Q��");
        scanf("%s",stu1->qq);

        printf("��ַ");
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
        printf("\n��Ҫ������~(y/n)");
        fflush(stdin);
        choice=getchar();

        if(toupper(choice)!='Y')
        {
            fclose(fp);
            printf("\n��������������������~");
            getch();
            return;

        }
        system("cls");

    }
    while (1);

}


void findstu(STU *stu_first)    //��ѯͬѧ��Ϣ
{
    int choice,ret=0,num;
    char str[13];
    STU *stu1;

    system("cls");

    do
    {
        printf("\t��ѯѧ����Ϣ\n");
        bound('_',25);
        printf("\t1.��������ѯ\n");
        printf("\t2.��ѧ�Ų�ѯ\n");
        printf("\t3.���绰��ѯ\n");
        printf("\t4.��QQ��ѯ\n");
        printf("\t0.�������˵�\n");
        bound('_',25);
        printf("\n��ѡ��˵�Ӵ��");

        do
        {
            fflush(stdin);
            choice=getchar();
            system("cls");

            switch(choice)
            {
            case'1':
                printf("����Ҫ��ѯ��ͬѧ��������");
                scanf("%s",str);

                stu1=findname(str,stu_first);
                displaystu(stu1,"����",str);
                getch();
                break;

            case'2':
                printf("����Ҫ��ѯ��ͬѧ��ѧ�ţ�");
                scanf("%s",str);

                stu1=findnum(str,stu_first);
                displaystu(stu1,"ѧ��",str);
                getch();
                break;

            case'3':
                printf("����Ҫ��ѯ��ͬѧ�ĵ绰��");
                scanf("%s",str);

                stu1=findtelephone(str,stu_first);
                displaystu(stu1,"�绰",str);
                getch();
                break;

            case'4':
                printf("����Ҫ��ѯ��ͬѧ��Q�ţ�");
                scanf("%s",str);

                stu1=findqq(str,stu_first);
                displaystu(stu1,"Q��",str);
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

STU  *findname(char *name,STU *stu_first)         //�������������в�ѯ
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

STU *findnum(char *name,STU *stu_first)       //��ѧ�Ų�ѯ
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

STU *findtelephone(char *name,STU *stu_first)     //���绰��ѯ
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

STU *findqq(char *name,STU *stu_first)         //��q�Ų�ѯ
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
        printf("\n%s:%s��Ϣ���£�\n",field,name);
        bound('_',20);
        printf("ѧ��:%s\n",stu->num);
        printf("רҵ:%s\n",stu->pro);
        printf("����:%s\n",stu->name);
        printf("�Ա�:%c\n",stu->sex);
        printf("����:%d\n",stu->chinese);
        printf("��ѧ:%d\n",stu->math);
        printf("Ӣ��:%d\n",stu->english);
        printf("�绰:%s\n",stu->tel);
        printf("Q��:%s\n",stu->qq);
        printf("��ַ:%s\n",stu->address);
        bound('_',20);
    }
    else
    {
        bound('_',20);
        printf("\nϵͳ��û��%sΪ��%s��ͬѧ����Ϣ��~\n",field,name);
    }
    return;
}

void liststu(STU *stu_first)    //��ʾͬѧ��Ϣ
{
    STU *stu1;
    printf("\nͬѧ�б�\n");
    bound('_',80);
    stu1=stu_first;

    while(stu1)
    {
        printf("ѧ��:%s",stu1->num);
        printf("רҵ:%s",stu1->pro);
        printf("����:%s",stu1->name);
        printf("�Ա�:%c",stu1->sex);
        printf("����:%d",stu1->chinese);
        printf("��ѧ:%d",stu1->math);
        printf("Ӣ��:%d",stu1->english);
        printf("�绰:%s",stu1->tel);
        printf("Q��:%s",stu1->qq);
        printf("��ַ:%s\n",stu1->address);
        bound('_',80);
        stu1=stu1->next;
    }

    printf("\n�����������~\n");
    getch();
    return;
}

void modifystu(STU *stu_first, char *gsave)   //�޸�ͬѧ��Ϣ
{
    STU *stu1;
    char name[10] ,*str;
    int choice;

    printf("\n����Ҫ�޸ĵ�ͬѧ��������");
    scanf("%s",name);

    stu1=findname(name,stu_first);
    displaystu(stu1,"����",name);

    if(stu1)
    {
        printf("\nѡ��Ҫ�޸ĵ���Ŀ��~\n");
        bound('_',35);
        printf("1.�޸����ĳɼ�    2.�޸���ѧ�ɼ�\n");
        printf("3.�޸�Ӣ��ɼ�    4.�޸ĵ绰\n");
        printf("5.�޸�q��         6.�޸ĵ�ַ\n");
        printf("0.����~\n");
        bound('_',35);
        printf("\n��ѡ��");
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
                str=modi_field("�绰",stu1->tel,12);
                if(str!=NULL)
                {
                    strcpy(stu1->tel,str);
                    free(str);
                }

            case'5':
                str=modi_field("Q��",stu1->qq,10);
                if(str!=NULL)
                {
                    strcpy(stu1->qq,str);
                    free(str);
                }

            case'6':
                str=modi_field("��ַ",stu1->address,30);
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
        printf("�޸����~�����������\n");
        getch();
    }
    return;
}

int modi_grade(int n)
{
    int grade;

    printf("ԭ���ĳɼ���%d\n",n);
    printf("�µĵĳɼ���");
    scanf("%d",&grade);

    return grade;
}


char *modi_field(char *field,char *s,int n)
{
    char *str;
    str=malloc(sizeof(char)*n);
    if(str==NULL);
    {
        printf("�ڴ����ʧ�ܿ�~����������ع�~\n");
        getch();
        return NULL;
    }

    printf("ԭ%s:%s\n",field,s);
    printf("�޸�Ϊ��������%d���ַ�)",n );
    scanf("%s",str);

    return str;
}

void delstu(STU *stu_first, char *gsave)       //ɾ��ͬѧ��Ϣ
{
    int i=0,find=0;
    STU *stu1,*stu2;
    char name[10],choice;

    system("cls");
    printf("\n����Ҫɾ����ͬѧ������");
    scanf("%s",name);

    stu1=stu_first;
    stu2=stu1;
    while(stu1)
    {
        if(strcmp(stu1->name,name)==0)
        {
            find=1;
            system("cls");

            printf("ѧ����%s��Ϣ���¹�~��\n",stu1->name);
            bound('_',20);
            printf("ѧ��:%s",stu1->num);
            printf("רҵ:%s",stu1->pro);
            printf("����:%s",stu1->name);
            printf("�Ա�:%c",stu1->sex);
            printf("����:%d",stu1->chinese);
            printf("��ѧ:%d",stu1->math);
            printf("Ӣ��:%d",stu1->english);
            printf("�绰:%s",stu1->tel);
            printf("Q��:%s",stu1->qq);
            printf("��ַ:%s",stu1->address);
            bound('_',20);
            printf("���Ҫɾ����ͬѧ����Ϣ��~��y/n)");

            fflush(stdin);
            choice=getchar();

            if(choice!='y'&&choice!='Y')
                return;
            if(stu1==stu_first)
                stu_first=stu1->next;
            else stu2->next=stu1->next;

            free(stu1);
            (*gsave)=1;
            savedata(gsave, stu_first);      //��������
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
        printf("��~û�ҵ���Ϊ%s��ͬѧ��~TA����Ϣ�ǲ��Ǳ��ڵĴε�����~\n",name);
        getch();
    }
    return;
}

void resetpwd(char password[])          //��������
{
    char pwd[9],pwd1[9],ch;
    int i;
    FILE *fp1;

    system("cls");

    printf("\n����������~");
    for(i=0; i<8&&((pwd[i]=getch())!=13); i++)
        putch('*');

    pwd[i]='\0';
    if(strcmp(password,pwd)!=0)
    {
        printf("������˴��ֵܣ����������ȥ��~\n");
        getch();
        return;
    }
    do
    {
        printf("\n����������Ӵ�����8λ~����");
        for(i=0; i<8&&((pwd[i]=getch())!=13); i++)
            putch('*');

        printf("\nȷ���������~");
        for(i=0; i<8&&((pwd1[i]=getch())!=13); i++)
            putch('*');

        pwd[i]='\0';
        pwd1[i]='\0';
        if ((strcmp(pwd,pwd1))!=0)
            printf("\n�����������벻ͬ�����ֵܣ������������~\n") ;
        else break;
    }
    while(1);

    if((fp1=fopen("config.dat","wb"))==NULL)
    {
        printf("\n����ϵͳ�����ļ�ʧ�ܿ�~���������ȥ��С����\n");
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
    printf("\n�޸�����ɹ���Ӵ~�����������~\n");
    getch();
    return;
}
void summarystu(STU* stu_first)      //����ͳ��
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

        if(stu1->sex=='M'||stu1->sex=='m')  //ע��1.�ַ���ֱ�ӱȽ� 2.�ַ�Ҫ��'' 3.��==
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

    printf("���ļ�������%d",pass_sum_chi);
    printf("������������%d",ecl_sum_mat);
    printf("��ѧ��������%d",pass_sum_mat);
    printf("��ѧ��������%d",ecl_sum_mat);
    printf("Ӣ����������%d",pass_sum_eng);
    printf("Ӣ����������%d",ecl_sum_eng);

    printf("��������%d",man);
    printf("Ů��%d",woman);
    return;
}





