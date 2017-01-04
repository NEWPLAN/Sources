#include<string.h>
#include<stdio.h>
#include<malloc.h>
#include<conio.h>
#include<math.h>
#define MAX 100    /**最多能输入的结点数**/
#define FALSE  0
#define TRUE  0
#define LH +1
#define EH 0
#define RH -1
#define M 1000
typedef struct treenode /*链表定义结点类型*/
{
    int data;/*输入的数列的值为整数*/
    struct treenode *lchild,*rchild;
    int bf;   /*结点的平衡因子*/
} treenode,*tree;
typedef struct /*顺序表类型的结点的数据类型*/
{
    int data;/*输入的数列的值为整数*/
    int bh;   /*结点的序号*/
} treenode1,*tree1;
typedef struct     /*对栈的结构描述*/
{
    tree *base,*top;
    int stacksize;
} sqstack;

typedef struct /*顺序表结构的栈的数据结构*/
{
    tree1 *base,*top;
    int stacksize;
} sqstack1;
tree T=NULL,R;  /******链表指向根节点的指针设为全局变量******/
tree1 G; /*顺序表类型的指向根结点的指针*/
tree settree(int a[],int );
/*a[]存放最原始的数列，b[]存放经过处理后的数列，k表示当前数列的大小*/
int a[MAX],b[MAX],k=0;
void R_Rotate();
void L_Rotate();
void LeftBalance();    /***左平衡旋转*****/
void RightBalance();   /****右平衡旋转****/
int InsertAVL();
tree settree(int f[],int c)   /*建树函数*/
{
    int i;
    tree r,q,t;
    r=(tree)malloc(sizeof(treenode));  /*开始建立二叉排序树*/
    r->data=f[1];
    r->lchild=r->rchild=NULL;
    for(i=2; i<=c; i++)
    {
        q=r;
        t=q;
        while(q!=NULL)
        {
            if(f[i]>q->data)
            {
                t=q;
                q=q->rchild;
            }
            if(f[i]<q->data)
            {
                t=q;
                q=q->lchild;
            }
            if(f[i]==q->data)break;
        }
        if(f[i]==t->data);
        else
        {
            q=(tree)malloc(sizeof(treenode));
            q->data=f[i];
            q->lchild=q->rchild=NULL;
            if(f[i]>t->data)
            {
                t->rchild=q;
            }
            if(f[i]<t->data)
            {
                t->lchild=q;
            }
        }
    }    /*二叉排序树建立完成*/
    return r;
}
/*中序遍历函数*/
Inorder(tree t)
{
    tree p;
    sqstack *s;
    if(t==NULL)
    {
        printf("please input the array first!");
        return;
    }
    s=(sqstack *)malloc(sizeof(sqstack));
    initstack(s);
    printf("nnThe result of BIANLI is:");
    p=t;
    while(p||!stackempty(s))
    {
        if(p)
        {
            push(s,p);
            p=p->lchild;
        }
        else
        {
            pop(s,&p);
            if(!visit(p->data))
            {
                error2(); /*************错误2的显示************/
                return;
            }
            p=p->rchild;
        }
    }
    return;
}
/*构造空栈*/
initstack(sqstack *s)
{
    (*s).base=(tree *)malloc(100*sizeof(tree));
    if(!(*s).base)
    {
        error1();/************用来显示内存不足***********/
        exit(0);
    }
    (*s).top=(*s).base;
    (*s).stacksize=100;
    return 1;
}
/*****是否为空栈*******/
stackempty(sqstack *s)
{
    if((*s).base==(*s).top) return 1;
    else return 0;
}
/********入栈********/
push(sqstack *s,tree p)
{
    if((*s).top-(*s).base>=(*s).stacksize)
    {
        (*s).base=(tree *) realloc ((*s).base,((*s).stacksize+10) * sizeof(tree));
        if(!(*s).base)
        {
            error1();
            exit(0);
        }
        (*s).top=(*s).base+(*s).stacksize;
        (*s).stacksize+=10;
    }
    else
    {
        *((*s).top)=p;
        (*s).top++;
        return;
    }
}
/********出栈***********/
pop(sqstack *s,tree *p)
{
    if((*s).top==(*s).base)
    {
        error2();
        return;
    }
    --((*s).top);
    *p=(*(s->top));
    return;
}
/*******显示结点数据*********/
visit(int n)
{
    if(n<0)return 0;
    else
    {
        printf("%6d",n);
        return 1;
    }
}
/*******计算平均查找长度********/
jisuan(int c[],int m,tree p)
{
    int b=0,i,s=1;/***存放查找当前数的长度***/
    float g=0;/****平均查找长度***/
    tree q=p;
    for(i=1; i<=m; i++)
    {
        do
        {
step2:
            if(c[i]==q->data)
            {
                g=g+s;
                b++;
                q=p;
                s=1;
                break;
            }
            if(c[i]<q->data)
            {
                q=q->lchild;
                s++;
                goto step2;
            }
            if(c[i]>q->data)
            {
                q=q->rchild;
                s++;
                goto step2;
            }
        }
        while(b!=m);
    }
    g=g/m;
    printf("nnThe length of average is: %f",g);
    return;
}
/******用于显示内存不足*****/
error1()
{
    printf("n neicun bu zu!!n");
}
/********显示错误*******/
error2()
{
    printf("n error!!n");
}
/*************删除函数*****************/
delete1(tree p,int de)
{
    tree q=p,t,f;
    int m=0;/***用于记录方向*****/
    if(p->data==de)     /*如果是要删除根结点*/
    {
        if(p->lchild==NULL)
        {
            T=p->rchild;
            free(p);
        }
        else
        {
            if(p->rchild==NULL)
            {
                T=p->lchild;
                free(p);
            }
            else
            {
                q=p->lchild;
                t=q;
                T=q;
                if(t->rchild!=NULL)t=t->rchild;
                t->rchild=p->rchild;
                free(p);
                return;
            }
        }
    }  /*end if*/
    while(p!=NULL)
    {
        if(de==p->data)break;
        if(de>p->data)
        {
            f=p;
            p=p->rchild;
            m=1;
        }
        if(de<p->data)
        {
            f=p;
            p=p->lchild;
            m=0;
        }
    }
    if((p->lchild==NULL)&&(p->rchild==NULL))
    {
        free(p);
        if(m==0)
        {
            f->lchild=NULL;
            return;
        }
        if(m==1)
        {
            f->rchild=NULL;
            return;
        }
    }
    if(p->lchild==NULL)
    {
        if(m==0)
        {
            f->lchild=p->rchild;
            free(p);
            return;
        }
        if(m==1)
        {
            f->rchild=p->rchild;
            free(p);
            return;
        }
    }
    if(p->rchild==NULL)
    {
        if(m==0)
        {
            f->lchild=p->lchild;
            free(p);
            return;
        }
        if(m==1)
        {
            f->rchild=p->lchild;
            free(p);
            return;
        }
    }
    t=p->lchild;
    if(t->rchild!=NULL)t=t->rchild;
    f->lchild=p->lchild;
    t->rchild=p->rchild;
    free(p);
    return;
}
/*********************打印树****************/
dayin(tree t,int c[],int m)
{
    int X,Y,i,b=0,s=1,x,y;
    tree q=t;
    printf("nThe tree is:n");
    X=wherex();
    X=X+50;
    Y=wherey();
    x=X;
    y=Y;
    for(i=1; i<=m; i++)
    {
        do
        {
step2:
            if(c[i]==q->data)
            {
                b++;
                gotoxy(x,y);
                printf("%d",q->data);
                x=X;
                y=Y;/*X Y里面存放根结点的坐标，每显示完一个结点后便回到根结点处*/
                s=1;
                q=t;
                break;
            }
            if(c[i]<q->data)
            {
                q=q->lchild;
                s++;
                x=x-(m-s+2);
                y=y+1;  /*若向左，则向左空出相应的列出来，行数加一*/
                goto step2;
            }
            if(c[i]>q->data)
            {
                q=q->rchild;
                s++;
                x=x+(m-s+2);
                y=y+1;
                goto step2;
            }
        }
        while(b!=m);
    }
    printf("n");
    return;
}
/**********************欢迎**************/
welcome()
{
    int i;
    clrscr();
    gotoxy(1,10);
    for(i=1; i<=80; i++)printf("*");
    gotoxy(1,14);
    for(i=1; i<=80; i++)printf("*");
    gotoxy(38,12);
    printf("Welcome!");
    gotoxy(32,18);
    printf("This program is made by:ZhouFeng   012004017415");
    gotoxy(55,19);
    printf(" ZhouFangFu 012004017406");
    gotoxy(64,20);
    printf("CS0414");
    getch();
    clrscr();
}
/*对以*p为根的二叉排序树作右旋处理,处理之后p指向新的根节点,即旋转处理之前的左子树的根节点*/
void R_Rotate(tree * p)
{
    tree lc;
    lc = (*p)->lchild;              /*lc指向p左子树根节点*/
    (*p)->lchild = lc->rchild;      /*lc的右子树挂接为p的左子树*/
    lc->rchild = *p;
    *p = lc;                         /*p指向新的根节点*/
}
/*对以*p为根的二叉排序树作左旋处理,处理之后p指向新的根节点,即旋转处理之前的右子树的根节点*/
void L_Rotate(tree * p)
{
    tree rc;
    rc = (*p) -> rchild;
    (*p) -> rchild = rc -> lchild;
    rc -> lchild = (*p);
    *p = rc;
}
/*若在平衡的二叉排序树中不存在和data有相同关键字的结点，则插入一个数据元素为data
的新结点，并返回1，否则返回0。若因插入而使二叉排序树失去平衡，则作平衡旋转处理，变量
taller反映树长高与否*/
int InsertAVL(tree * t,int data, int * taller )
{
    if(!(*t))         /*插入新结点，树长高*/
    {
        *t = (tree)malloc(sizeof(treenode));
        (*t) -> data = data;
        (*t) -> lchild = (*t) -> rchild = NULL;
        (*t) -> bf = EH;
        *taller = TRUE;
    }
    else
    {
        if(data == (*t)->data)
        {
            *taller = FALSE;    /*树中已存在和data有相同关键字的结点则不再插入*/
            return  0;
        }
        if(data < (*t) -> data) /*应继续在*t的左子树中进行搜索*/
        {
            if(!InsertAVL(&(*t)->lchild,data,taller))return 0; /*未插入*/
            if(taller)          /*已插入到*t的左子树中且左子树“长高”*/
                switch((*t) ->bf) /*检查*t的平衡度*/
                {
                case LH:  /*原本左子树比右子树高，需要作左平衡处理*/
                    LeftBalance(t);
                    *taller = FALSE;
                    break;
                case EH: /*原本左右子树等高，现因左子树增高而使树增高*/
                    (*t) -> bf = LH;
                    *taller = TRUE;
                    break;
                case RH:  /*原本右子树比左子树高，现在左右子树等高*/
                    (*t) -> bf = EH;
                    *taller = FALSE;
                    break;
                }
        }
        else          /*应继续在*t的右子树中进行搜索*/
        {
            if(!InsertAVL(&(*t) -> rchild,data,taller))return 0;  /*未插入*/
            if(taller)            /*已插入到*t的右子树中且右子树“长高”*/
                switch((*t) -> bf)   /*检查*t的平衡度*/
                {
                case LH:  /*原本左子树比右子树高，现在左右子树等高*/
                    (*t) -> bf = EH;
                    *taller = FALSE;
                    break;
                case EH:  /*原本左右子树等高，现因右子树增高而使树增高*/
                    (*t) -> bf = RH;
                    *taller = TRUE;
                    break;
                case RH:  /*原本右子树比左子树高，需要作右平衡处理*/
                    RightBalance(t);
                    *taller = FALSE;
                    break;
                }/*switch*/
        }/*else*/
    }/*else*/
    return 1;
}/*InsertAVL*/
