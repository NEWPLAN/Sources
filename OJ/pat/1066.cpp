#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int Elemtype;

typedef struct Balanced_Binary_Tree
{
    Elemtype e; /*Ԫ�ص�����*/
    int bf;		/*balance factor*/
    struct Balanced_Binary_Tree *child[2]; /*���Һ���*/
}*AVL;

///------------�򵥵�λ����-------------------
void setbit(char *i,char val,char pos)
{
    if(pos==1)
        (*i)=(*i)|1;
    else
    {
        if(val==1)    (*i)=(*i)|2;
        else    (*i)=(*i)&1;
    }
}
char getbit(char i,char pos)
{
    ///����ʱ�����������ܷ���2///
//    return (i&pos); ����ĵط�
    return (i&pos)&&1;
    /////////////////////////////
}
///--------------------------------------------

///-----------����һ�����---------------------
AVL createnode(Elemtype e)
{
    AVL node=NULL;

    node=(AVL)malloc(sizeof(struct Balanced_Binary_Tree));
    node->e=e;    node->bf=0;
    node->child[0]=node->child[1]=NULL;
    
    return node;
}
///---------------------------------------------

///���������AVL�ĺ��Ĳ����������������
///���������ﱣ��ƽ����������������

//�ı����Ӻ���
void setfactor(AVL f,int button)
{
    char fir=button/10,sec=button%10;
    AVL s=f->child[fir],ss=s->child[sec];
    char choice=ss->bf;
    int a=1,b=0;

    //////////����ʱ���֣�ɾ��ʱ���������/////////////
/////����ʱ�������������������button=0,��s->bf=1//
/////��button=11����s->bf=-1��Ȼ��ɾ��ʱ��ȴ�����/
/////button=0����button=11ʱ s->bf=0!!!!!!!////////
/////��ô�������������ƽ������õ����Ӿ͸�һ���//
/////��һ����!!!����///////////////////////////////
    if(button==0 && s->bf==0)    f->bf=1,s->bf=-1;
    else if(button==11 && s->bf==0)    f->bf=-1,s->bf=1;
    ///////////////////////////////////////////////////
    else if(button==0 || button==11)
    {
        f->bf=0;
        s->bf=0;
    }
    else
    {
        /////д����ʱ����������������///////////////////
    //    if(button==1)    choice=-choice;
        /////����Ϊʲô�Ҳ��Ե�ʱ����ô����?!///////////
/////���ٴβ��ԣ��ϱ�ȷʵ����!!!////////////////
/////��Ϊ�±�Ӧ�þͶ��˰�///////////////////////
        if(button==1)    {a^=b,b^=a,a^=b;}
        ////////////////////////////////////////////////

        if(choice==-1)    f->bf=a,s->bf=b;
        else if(choice==0)    f->bf=s->bf=0;
        else    f->bf=-b,s->bf=-a;
        
        ss->bf=0;
    }
}
//���ڵ�任����
void conversion(AVL *T,char direction)
{
    AVL f=*T,s=f->child[direction];

    f->child[direction]=s->child[!direction];
    s->child[!direction]=f;
    *T=s;
}
//����ƽ�⺯��
void keepbalance(AVL *T,char fir,char sec)
{
    AVL *s=&((*T)->child[fir]);
    int button=fir*10+sec;

    if(button==0 || button==11)
    {
        setfactor((*T),button);
        conversion(T,fir);
    }
    else
    {
        setfactor((*T),button);
        conversion(s,sec);
        conversion(T,fir);
    }
}
///���������������������������

///------------����ʱ��ѡ�����-------------------
void selectforInsert(AVL *T,char *info,int direction)
{
    AVL cur=*T;
    char firdirection,secdirection;

    if(direction==0)    (cur->bf)++;
    else    (cur->bf)--;

    if(cur->bf==0)    setbit(info,1,1);
    else if(cur->bf==-1 || cur->bf==1)    setbit(info,direction,2);
    else
    {        
        firdirection=direction;
        secdirection=getbit(*info,2);
        keepbalance(T,firdirection,secdirection);
        setbit(info,1,1);
    }
}
//----------------------------------------------

//*************�������************************//
char InsertAVL(AVL *T,Elemtype e)
{                                //�����ڲ�ѯ
    char info;
    
    if(!(*T))
    {
        (*T)=createnode(e);
        return 0;
    }
    else if((*T)->e==e)        return -1;
    else if((*T)->e>e)//��
    {
        info=InsertAVL(&((*T)->child[0]),e);

        if(getbit(info,1))    return info;
        
        selectforInsert(T,&info,0);
    }
    else              //��
    {
        info=InsertAVL(&((*T)->child[1]),e);

        if(getbit(info,1))    return info;

        selectforInsert(T,&info,1);
    }
    return info;
}
//*********************************************//

//-------------ɾ��ʱ��ѡ�����--------------------
void selectforDelete(AVL *T,char *info,char direction)
{
    AVL cur=(*T);
    char firdirection,secdirection;

    if(direction==0)    (cur->bf)--;
    else    (cur->bf)++;
    
    if(cur->bf==0)    *info=0;
    else if(cur->bf==-1 || cur->bf==1)    *info=1;
    else
    {
        firdirection=!direction;
        ///����ʱ������������д��һ���Ⱥ�////////////////////
//        if(cur->child[firdirection]->bf=1)    secdirection=0;�ݣ���˧��!ԭ��1==a����дȷʵ�б�Ҫ!
        if(1==cur->child[firdirection]->bf)    secdirection=0;
        /////////////////////////////////////////////////////
        else    secdirection=1;
        keepbalance(T,firdirection,secdirection);

        /////////////////////////////////////////////////////////////////////////////////////////
///����ʱ�����־�������ƽ�������*info��һ������0�������Ǹ������������setfactor��/////
///�������������ʱ������*infoӦ��Ϊ1! ���Դ�������£�//////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
    //    *info=1; д����ʱ���������ɲ�һ����...������ƽ���ˣ������ڵ�����ӱȱ�!
//    *info=0;//��ˣ��⻹û�껹Ҫ��0!! ............���������ﲻһ����0! 
////�����Ǹ����������Ĺ�!//
        if(cur->bf==0)    *info=0;
        else    *info=1;
        /////////////////////////////////////////////////////////////////////////////////////////
    }
}
//------------------------------------------------

//-------------����ݹ�--����ɾ��-----------------
char find(AVL *gogal,AVL *p)
{
    char info;
    AVL tp=NULL;
    
    if(NULL!=(*p)->child[0])
    {
        info=find(gogal,&((*p)->child[0]));
        if(info!=0)    return info;
        selectforDelete(p,&info,0);
    }
    else
    {
        (*gogal)->e=(*p)->e;
        tp=(*p)->child[1];
        free((*p));
        *p=tp;
        info=0;
    }
    return info;
}
//------------------------------------------------

//**************ɾ������*************************//
char DeleteAVL(AVL *T,Elemtype e)
{
    char info;
    AVL tp=NULL;

    if(!(*T))    return -1;//ԭif(!T)    return -1;��2011��11��29��8:59:15�޸�
    else if((*T)->e==e)
    {
        if(NULL!=(*T)->child[1])
        {
            info=find(T,&((*T)->child[1]));
            if(info!=0)    return info;
            selectforDelete(T,&info,1);
        }
        else
        {
            //////////////����ʱ����������д����!!!///////////////////////////////////////
        //    (*T)=(p=(*T)->child[0])-(free((*T)),0);//Just save a variable! ���������
//    (*T)=p-(free((*T)),0); ����
//    (*T)=(p=((*T)->child[0]))+(free((*T)),0); ������
            tp=((*T)->child[0]);
            free((*T));
            *T=tp;
            //����ʱ����������©�˸�info��ֵ
            info=0;
            ///////////////////////////////////////////////////////////////////////////////
        }
    }
    else if((*T)->e>e)
    {
        info=DeleteAVL(&(*T)->child[0],e);
        if(info!=0)    return info;
        selectforDelete(T,&info,0);
    }
    else
    {
        info=DeleteAVL(&(*T)->child[1],e);
        if(info!=0)    return info;
        selectforDelete(T,&info,1);
    }
    return info;
}
//************************************************//


//*****************JUST FOR TEST************************//
#define MOVE(x)    (x=(x+1)%1000)
AVL queue[1000];

void print(AVL p,int i)
{
    int front,rear,temp,count;

    front=rear=-1; count=temp=0;
    queue[MOVE(rear)]=p; count++;
    
    printf("%d\n",i);
    while(front!=rear)
    {
        p=queue[MOVE(front)];    count--;
        
        
        if(p->child[0])    queue[MOVE(rear)]=p->child[0],temp++;
        if(p->child[1])    queue[MOVE(rear)]=p->child[1],temp++;
        
        printf("%d->%d ",p->e,p->bf);
        if(count==0)
        {
            printf("\n");
            count=temp;
            temp=0;
        }    
    }
    printf("\n");
}
//**************************************************//


int main()
{
    AVL T=NULL;
    int i,nodenum=0;
	int j;
    nodenum=100;
	scanf("%d",&nodenum);
    for(i=0;i<nodenum;i++)
    {
		scanf("%d",&j);
        InsertAVL(&T,j);
    }
	printf("%d",T->e);
    return 0;
}