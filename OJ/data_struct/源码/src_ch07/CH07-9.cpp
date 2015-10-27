/*********************************************
*  算法7.9  链式基数排序算法
**********************************************/

#define RADIX 10  // RADIX为关键字基数，整数时为0-9

typedef struct Node{
    int data;
    Node *next;
}Node, *NodePtr;

typedef struct{
    Node *front;
    Node *rear;
}TPtrs;

/**************************************************
 * 基数排序的分配过程：对链表phead中元素按第idx个关键字
 * 建立子序列（链队列），使得同一子序列中的关键字值相等
 * 对于第idx轮分配，链表已按关键字key[0], key[1],…, key[idx-1]有序
 * Que[k].front 和Que[k].rear分别指向第k个队列的第一个和最后一个数据
***************************************************/
void radixsort_distribute(NodePtr phead, TPtrs Que[], int idx){
    int k, pos;
    NodePtr ptr = NULL;
    
    for( k = 0; k < RADIX; ++k ){ // 清空链队列的头尾指针数组
       Que[k].front = NULL; 
       Que[k].rear = NULL;
    }

    // 取出原始链队列中第一个数据结点
    ptr = phead->next;   
    if(ptr){
        phead->next = ptr->next;
        ptr->next = NULL;
    }

    while(ptr){
        // 取出结点*ptr中数据部分的第idx位关键字，作为访问Que数组的下标
        pos = get_digit(ptr->data, idx); 
        
        // 将结点*ptr分配到相应队列中
        if( Que[pos].front == NULL){
            Que[pos].front = ptr;
            Que[pos].rear = ptr;
        }
        else{
            Que[pos].rear->next = ptr;
            Que[pos].rear = ptr;
        }

        // 取出原始链队列中下一个数据结点
        ptr = phead->next; 
        if(ptr){
            phead->next = ptr->next;
            ptr->next = NULL;
        }
    }
}

/**************************************************
 * 基数排序的收集过程：将Que[0], Que[1],…, Que[RADIX-1]
 * 所指向的链队列元素收集起来形成新的链表
***************************************************/
void radixsort_collect(NodePtr phead, TPtrs Que[])
{
    int i;
    NodePtr pend;
    
    // 采用for循环找出Que数组中第一个指向非空队列的元素
    for(i = 0; !Que[i].front; ++i);
    
    // 将第一个非空队列链入到新的链表中
    phead->next = Que[i].front;
    pend = Que[i].rear;
    i++;

    // 寻找其余非空队列，并将其顺序链接到主队列
    for(; i < RADIX; ++i){
        if(Que[i].front){
            pend->next = Que[i].front;
            pend = Que[i].rear;
        }
    }
    pend->next = NULL;  // 修改链队列尾结点的next指针
}

/**************************************************
 * 对正整数构成的数组R执行基数排序
***************************************************/
void sort_radix(int R[], int num){
    int i;
    NodePtr ptr;
    // 建立指向链队列头尾结点的指针数组
    TPtrs Que[RADIX]; 

    // 求序列中的最大值（用于确定关键字个数d）
    int max_val = findmax(R, num);

    // 从原始序列出发构建辅助链队列
    NodePtr phead = build_chain(R, num);

    // 基数排序：多轮分配－收集操作
    // 迭代次数：由关键字位数决定
    for( i = 0;  max_val;  max_val/=10, ++i){
        radixsort_distribute(phead, Que, i);
        radixsort_collect(phead, Que);
    }

    // 将排序结果写入原始数组R
    ptr = phead->next;
    for(i = 0; i < num; ++i){
        if(ptr){
            R[i] = ptr->data;
            ptr = ptr->next;
        }        
    }
    // 销毁辅助链队列
    destroy_chain(phead);
}

int main()
{
    int i, num, len;
    int keys[] = {209, 386, 768, 185, 247, 606, 230, 834, 539 };

    num = sizeof(keys)/sizeof(keys[0]);

    sort_radix(keys, num);

    printf("**********************************************\n");
    printf("最终的排序结果：\n");
    printf("**********************************************\n");
    for(i = 0; i < num; ++i){
        printf("%2d\n", keys[i]);        
    }

    system("pause");
    return 0;
}

/**************************************************
 * 一些辅助函数的定义如下
***************************************************/

// 取出整数val的第idx位（由低到高，个位的idx == 0）
int get_digit(int val, int idx)
{
    int i;
    for(i = 0; i < idx; ++i){
        val = val / 10;
    }
    return (val%10);
}

// 找出数组R中的最大值（作为返回值）
int findmax(int R[], int num)
{
    int i, maxval = 0xFFFFFFFF;
    for(i = 0; i < num; ++i ){
        if(maxval < R[i]){
            maxval = R[i];
        }
    }
    return maxval;
}


// 根据数组R构建带头结点的单链表（返回值为指向头结点的指针）
NodePtr build_chain(int R[], int num)
{
    int i;
    NodePtr phead = (NodePtr)malloc(sizeof(Node));
    if(!phead){
        printf("Error: 为头结点分配内存失败！\n");
        exit(0);
    }
    phead->next = NULL;
    for(i = 0; i < num; ++i ){
        NodePtr pnode = (NodePtr)malloc(sizeof(Node));
        if(!pnode){
            printf("Error: 为数据结点%d分配内存失败！\n", R[i]);
            exit(0);
        }
        pnode->data = R[i];
        pnode->next = phead->next;
        phead->next = pnode;
    }
    return phead;
}
// 销毁头结点phead指向的单链表（释放内存空间）
void destroy_chain(NodePtr phead)
{
    NodePtr ptr;
    ptr = phead;
    while(phead != NULL){
        phead = ptr->next;
        free(ptr);
        ptr = phead;
    }
}

// 测试函数：顺序输出单链表phead中的元素值（参数idx表示轮数，默认值为0）
void output_chain(NodePtr phead, int idx = 0)
{
    printf("**********************************************\n");
    printf("第%d轮分配-收集后的排序结果：\n", idx+1);
    printf("**********************************************\n");
    while(phead->next){
        printf("%d\n", phead->next->data);
        phead = phead->next;
    }
}
