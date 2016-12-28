/*对以指针t所指结点为根的二叉树作左平衡旋转处理，本算法结束时，指针t指向新的根结点*/
void LeftBalance(tree *t)
{
    tree lc,rd;
    lc = (*t) -> lchild;
    switch(lc -> bf)
    {
        case LH:
             (*t) -> bf = lc ->bf = EH ;
             R_Rotate(t);break;
        case RH:
            rd = lc -> rchild;
            switch(rd ->bf)
            {
                case LH: (*t) -> bf = RH; lc -> bf = EH; break;
                case EH: (*t) -> bf = lc ->bf =EH;       break;
                case RH: (*t) -> bf = EH; lc -> bf = LH; break;
            }
            rd -> bf = EH;
            L_Rotate(&(*t)->lchild);
            R_Rotate(t);
    }/*switch(lc -> bf)*/
}/*LeftBalance*/
/*对以指针t所指结点为根的二叉树作右平衡旋转处理，本算法结束时，指针t指向新的根结点*/
void RightBalance(tree *t)
{
    tree rc,ld;
    rc = (*t) -> rchild;
    switch(rc -> bf)
    {
        case RH:
             (*t) -> bf = rc ->bf = EH ;
             L_Rotate(t);break;
        case LH:
            ld = rc -> lchild; 
            switch(ld ->bf)
            {
                case RH: (*t) -> bf = LH; rc -> bf = EH; break;
                case EH: (*t) -> bf = rc ->bf =EH;       break;
                case LH: (*t) -> bf = EH; rc -> bf = RH; break;
            }
            ld -> bf = EH;
            R_Rotate(&(*t)->rchild);
            L_Rotate(t);
    }/*switch(rc -> bf)*/
}
/*****************输入函数********************/
shuru()
{
int i=1,c; char x;
if(T!=NULL)
{
printf("The array is existed!nDo you want to input the array again?(Y/N)");
x=getch();printf("%c",x);
if((x=='y')||(x=='Y'))goto step2;else return;
}
step2:printf("nPlease input the array:n");
scanf("%d%c",&a[1],&x) ;
while(x!='n'){i++,scanf("%d%c",&a[i],&x);}
k=i;
} /*end shuru*/
/*******************对a[]进行处理，删除重复的数,k中存放数列的大小***********/
chuli()
{ int c=k,i,j,m;  m=1;
b[1]=a[1];
for(i=2;i<=c;)
{
for(j=1;j<=m;j++){if(a[i]==b[j])goto step1;}
m++;
b[m]=a[i];
step1:i++;
}
}/*end chuli*/
 /*************删除结点********************/
int del()
{
int de,i,j;
printf("nnPlease input the number which you want to delete:");
scanf("%d",&de);
for(i=1;i<=k;i++)
{
if(b[i]==de)
{printf("nThis node %d exist!      Now delete....n",de);
printf("The node %d has been deleted!n",de);
/********需要对数组重新处理********/
for(j=i;j<k;j++)
{
b[j]=b[j+1];
}
k=k-1;
getch();return de;
  } /*end if*/
}/*end for*/
printf("nThis node %d doesn't exist!n",de);
getch(); return 0;
}/*end del*/
/************插入函数*******************/
insert()
{
int in,i;   char x;
step1:
printf("nnplease input the node you want to insert:");
scanf("%d",&in);
if(k==0)
{printf("nInsert success!");b[1]=in;k++;}
else
{
for(i=1;i<=k;i++)
{
if(b[i]==in)
{printf("nThis node has existed!Please input again!");
for(i=1;i<=k;i++){printf("nb[%d]=%d",i,b[i]);}
printf("nDo you want to continue input?(Y/N)");
x=getch(); printf("%c",x);
if((x=='y')||(x=='Y'))goto step1;
else return;
}
}/*end for*/
/*把插入的数存放到数组b[]里面去*/
k++;b[k]=in;  printf("nInsert success!");
printf("nDo you want to continue input?(Y/N)");
x=getch(); printf("%c",x);
if((x=='y')||(x=='Y'))goto step1;
else return;
  } /*end else*/
}
 /****************建立平衡二叉树*******************/
avl()
{
int i,taller=0;
R=NULL;
for(i=1;i<=k;i++){InsertAVL(&R,b[i],&taller);}
}
/**********************顺序表存储结构的有关函数********************************/
/*顺序表中序遍历函数*/
Inorder1(tree1 t)
{
tree1 p;
sqstack1 *s;
s=(sqstack1 *)malloc(sizeof(sqstack1));
initstack1(s); /*先建栈*/
p=t;
printf("nnThe result is:");
while((p->data)||!stackempty1(s))
{
if(p->data)
{push1(s,p);
p=p+(p->bh);     }
else{pop1(s,&p);
if(!visit(p->data))
{error2();        /*************错误2的显示************/
return;}
p=p+(p->bh)+1;
}
}
return;
}
/*************构造空栈*************/
initstack1(sqstack1 *s)
{
(*s).base=(tree1 *)malloc(100*sizeof(tree1));
if(!(*s).base)
{
error1();      /************用来显示内存不足***********/
exit(0);
}
(*s).top=(*s).base;
(*s).stacksize=100;
return 1;
}
/*****是否为空栈*******/
stackempty1(sqstack1 *s)
{
if((*s).base==(*s).top) return 1;
else return 0;
}
 /********入栈********/
push1(sqstack1 *s,tree1 p)
{
if((*s).top-(*s).base>=(*s).stacksize)
{
(*s).base=(tree1 *) realloc ((*s).base,((*s).stacksize+10) * sizeof(tree1));
if(!(*s).base){error1();exit(0);}
(*s).top=(*s).base+(*s).stacksize;
(*s).stacksize+=10;
}
else
{
*((*s).top)=p;
(*s).top++;
return;}
}
/********出栈***********/
pop1(sqstack1 *s,tree1 *p)
{
if((*s).top==(*s).base)
{error2();return;}
--((*s).top);
*p=(*(s->top));
return ;
}
  /*************计算ASL*********************/
jisuan1(int c[],int m,tree1 p)
{
int b=0,i,s=1;/***存放查找当前数的长度***/
float g=0;/****平均查找长度***/
tree1 q=p;
for(i=1;i<=m;i++)
{
do{
step22:if(c[i]==q->data){g=g+s;b++;q=p;s=1;break;}
if(c[i]<q->data){q=q+q->bh;s++;goto step22;}
if(c[i]>q->data){q=1+q+q->bh;s++;goto step22;}
}while(b!=m);
}
g=g/m;
printf("nnThe length of average is: %f",g);
return;
}
/*******************删除结点的函数*******************/
delete2(int c[],int m,int de)
{int d[MAX];int i,k1,shu;
tree1 f,q,t;
/*把删除结点后的数列存在d[]里*/
for(i=1,k1=1;i<=m;i++)
{if(c[i]==de)continue;
else {d[k1]=c[i];k1++;}
}
m=m-1;
shu=pow(2,m+1)-1;
if(M<shu){printf("nNei cun bu zu!Error!");exit(1);}
f=(tree1)calloc(shu,sizeof(treenode1));
G=f;/*删除结点后指向根结点的指针*/
for(i=1,q=f;i<=shu;i++,++q){q->data=0;q->bh=0;}
f->bh=1;
f->data=d[1];
for(i=2;i<=m;i++)
{q=f;
while(q->data!=0)
{if(d[i]>q->data){t=q;q=1+q+(q->bh);}
 if(d[i]<q->data){t=q;q=q+(q->bh);}
}
q->data=d[i];
q->bh=(t->bh)+(q-t);
}
return;
}
/******************************************************************/
 /*****************程序说明*****************/
instruction()
{ int i;
gotoxy(1,3);
for(i=1;i<=30;i++)
printf("*");
printf("Program Instruction");
for(i=51;i<=80;i++)
printf("*");
gotoxy(3,5);
printf("1.This program has two parts.The storage structure of the first part is list.
     The other is array.");
gotoxy(3,10);
printf("2.When you input numbers,please use the space between each number,and end 
        with \n.");
gotoxy(3,15);
printf("3.This program is written by ZhouFeng & ZhouFangFu.");
gotoxy(3,19);
printf("4.If you have any question or advice,please send email to feng2211@163.com.");
gotoxy(33,24);printf("Copyright@2006");
getch();
}
/******************链表结构的界面**************/
lianbiao()
{
int i,de,j,temp,key;
tree t,e,q,r;
textmode(BW80);
clrscr();
T=NULL;
do{
clrscr();
gotoxy(28,8);printf ("1.Input the array ");
gotoxy(28,9);printf ("2 zhong xu BIANLI");
gotoxy(28,10);printf ("3 delete the node");
gotoxy(28,11);printf ("4 insert the node");
gotoxy(28,12);printf ("5 show the tree and output the ASL");
gotoxy(28,13);printf ("6 show the balance tree and the ASL");
gotoxy(28,14);printf ("7 Exit    ");
gotoxy(28,17);printf ("                      PLEASE CHOOSE :)");
scanf("%d",&key);
switch(key)
{  case 1:
          clrscr();shuru();chuli();T=settree(b,k);clrscr();   break;
   case 2:
          clrscr();
          if(T!=NULL)
                    {T=settree(b,k);Inorder(T) ;getch();clrscr();}
          else{printf("nplease input the array first!");getch();clrscr();}
          break;
   case 3:
          clrscr();
          if(T!=NULL)
                    {de=del(); T=settree(b,k);if(de!=0){delete1(T,de);} clrscr(); }
          else{printf("nplease input the array first!");getch();clrscr();}
          break;
   case 4:
          clrscr();
          if(T!=NULL)
                    {insert();T=settree(b,k); clrscr();}
          else{printf("nplease input the array first!");getch();clrscr();}
          break;
   case 5:
          clrscr();
          if(T!=NULL)
                    {dayin(T,b,k);jisuan(b,k,T);getch();clrscr();}
          else{printf("nplease input the array first!");getch();clrscr();}
          break;
   case 6:clrscr();
          if(T!=NULL)
                    {avl();dayin(R,b,k);jisuan(b,k,R);getch();clrscr();}
          else{printf("nplease input the array first!");getch();}
          break;
   case 7:break;
   default:gotoxy(28,18);printf("Error!Please choose again");getch(); break;
}/*end switch  */
}while(key!=7);
}
/***************顺序表结构的界面******************/
shunxu()
{
int a1[MAX],b1[MAX];
int i,j,c,k1,shu,de;
tree1 q,t,f;
/*输入数列*/
printf("nHow many numbers do you want to input?");
scanf("%d",&c);
printf("nPlease input the numbers:");
for(i=1;i<=c;i++)
{scanf("%d",&a1[i]);}
/*对a1[]数组里面的数进行处理，存放到b1[]*/
b1[1]=a1[1];
k1=1;
for(i=2;i<=c;)
{for(j=1;j<=k1;j++){if(a1[i]==b1[j])goto step11;}
k1++;
b1[k1]=a1[i];
step11:i++;
}
/*计算存储空间的大小*/
shu=pow(2,(k1)+1)-1;
if(M<shu){printf("nNei cun bu zu!Error!");exit(1);}
f=(tree1)malloc(shu*sizeof(treenode1));
G=(tree1)malloc(sizeof(treenode1));
G=f;
for(i=1,q=f;i<=shu;i++,++q){q->data=0;q->bh=0;}/*先把每个空间初始化*/
/*开始建树*/
f->bh=1;
f->data=b1[1];
for(i=2;i<=k1;i++)
{q=f;
while(q->data!=0)
{if(b1[i]>q->data){t=q;q=1+q+(q->bh);}
 if(b1[i]<q->data){t=q;q=q+(q->bh);}
}
q->data=b1[i];
q->bh=(t->bh)+(q-t);
}
/*进行中序遍历*/
Inorder1(G);
jisuan1(b1,k1,G);/***计算平均查找长度****/
/*输入想删除的结点*/
printf("nnPlease input the number which you want to delete:");
scanf("%d",&de);
for(i=1;i<=k1;i++)
{
if(b1[i]==de) /*如果该结点存在*/
{printf("nThis node %d exist!        Now delete....",de);
delete2(b1,k1,de); /**删除函数*/
Inorder1(G); /*再次进行中序遍历*/
break;}
if(i==k1)  /*如果该结点不存在*/
printf("nThis node %d doesn't exist!",de);
}
getch();
}
/******************主函数***************/
main()
{
int key1;
welcome();
textmode(BW80);
clrscr();
do{clrscr();
gotoxy(28,8);
printf("Please choose a storage type:");
gotoxy(28,10);
printf("1:lian biao");
gotoxy(28,11);
printf("2:shun xu biao");
gotoxy(28,12);
printf("3:instruction");
gotoxy(28,13);
printf("4:Exit");
gotoxy(40,14);
printf("PLEASE CHOOSE :)");
scanf("%d",&key1);
switch(key1)
{
case 1:clrscr();lianbiao();break;
case 2:clrscr();shunxu();break;
case 3:clrscr();instruction(); break;
case 4: break;
default:gotoxy(28,15);printf("Error!Please choose again");getch(); break;
}
}while(key1!=4) ;
}