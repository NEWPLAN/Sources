/*********************************************
*  �㷨7.9  ��ʽ���������㷨
**********************************************/

#define RADIX 10  // RADIXΪ�ؼ��ֻ���������ʱΪ0-9

typedef struct Node{
    int data;
    Node *next;
}Node, *NodePtr;

typedef struct{
    Node *front;
    Node *rear;
}TPtrs;

/**************************************************
 * ��������ķ�����̣�������phead��Ԫ�ذ���idx���ؼ���
 * ���������У������У���ʹ��ͬһ�������еĹؼ���ֵ���
 * ���ڵ�idx�ַ��䣬�����Ѱ��ؼ���key[0], key[1],��, key[idx-1]����
 * Que[k].front ��Que[k].rear�ֱ�ָ���k�����еĵ�һ�������һ������
***************************************************/
void radixsort_distribute(NodePtr phead, TPtrs Que[], int idx){
    int k, pos;
    NodePtr ptr = NULL;
    
    for( k = 0; k < RADIX; ++k ){ // ��������е�ͷβָ������
       Que[k].front = NULL; 
       Que[k].rear = NULL;
    }

    // ȡ��ԭʼ�������е�һ�����ݽ��
    ptr = phead->next;   
    if(ptr){
        phead->next = ptr->next;
        ptr->next = NULL;
    }

    while(ptr){
        // ȡ�����*ptr�����ݲ��ֵĵ�idxλ�ؼ��֣���Ϊ����Que������±�
        pos = get_digit(ptr->data, idx); 
        
        // �����*ptr���䵽��Ӧ������
        if( Que[pos].front == NULL){
            Que[pos].front = ptr;
            Que[pos].rear = ptr;
        }
        else{
            Que[pos].rear->next = ptr;
            Que[pos].rear = ptr;
        }

        // ȡ��ԭʼ����������һ�����ݽ��
        ptr = phead->next; 
        if(ptr){
            phead->next = ptr->next;
            ptr->next = NULL;
        }
    }
}

/**************************************************
 * ����������ռ����̣���Que[0], Que[1],��, Que[RADIX-1]
 * ��ָ���������Ԫ���ռ������γ��µ�����
***************************************************/
void radixsort_collect(NodePtr phead, TPtrs Que[])
{
    int i;
    NodePtr pend;
    
    // ����forѭ���ҳ�Que�����е�һ��ָ��ǿն��е�Ԫ��
    for(i = 0; !Que[i].front; ++i);
    
    // ����һ���ǿն������뵽�µ�������
    phead->next = Que[i].front;
    pend = Que[i].rear;
    i++;

    // Ѱ������ǿն��У�������˳�����ӵ�������
    for(; i < RADIX; ++i){
        if(Que[i].front){
            pend->next = Que[i].front;
            pend = Que[i].rear;
        }
    }
    pend->next = NULL;  // �޸�������β����nextָ��
}

/**************************************************
 * �����������ɵ�����Rִ�л�������
***************************************************/
void sort_radix(int R[], int num){
    int i;
    NodePtr ptr;
    // ����ָ��������ͷβ����ָ������
    TPtrs Que[RADIX]; 

    // �������е����ֵ������ȷ���ؼ��ָ���d��
    int max_val = findmax(R, num);

    // ��ԭʼ���г�����������������
    NodePtr phead = build_chain(R, num);

    // �������򣺶��ַ��䣭�ռ�����
    // �����������ɹؼ���λ������
    for( i = 0;  max_val;  max_val/=10, ++i){
        radixsort_distribute(phead, Que, i);
        radixsort_collect(phead, Que);
    }

    // ��������д��ԭʼ����R
    ptr = phead->next;
    for(i = 0; i < num; ++i){
        if(ptr){
            R[i] = ptr->data;
            ptr = ptr->next;
        }        
    }
    // ���ٸ���������
    destroy_chain(phead);
}

int main()
{
    int i, num, len;
    int keys[] = {209, 386, 768, 185, 247, 606, 230, 834, 539 };

    num = sizeof(keys)/sizeof(keys[0]);

    sort_radix(keys, num);

    printf("**********************************************\n");
    printf("���յ���������\n");
    printf("**********************************************\n");
    for(i = 0; i < num; ++i){
        printf("%2d\n", keys[i]);        
    }

    system("pause");
    return 0;
}

/**************************************************
 * һЩ���������Ķ�������
***************************************************/

// ȡ������val�ĵ�idxλ���ɵ͵��ߣ���λ��idx == 0��
int get_digit(int val, int idx)
{
    int i;
    for(i = 0; i < idx; ++i){
        val = val / 10;
    }
    return (val%10);
}

// �ҳ�����R�е����ֵ����Ϊ����ֵ��
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


// ��������R������ͷ���ĵ���������ֵΪָ��ͷ����ָ�룩
NodePtr build_chain(int R[], int num)
{
    int i;
    NodePtr phead = (NodePtr)malloc(sizeof(Node));
    if(!phead){
        printf("Error: Ϊͷ�������ڴ�ʧ�ܣ�\n");
        exit(0);
    }
    phead->next = NULL;
    for(i = 0; i < num; ++i ){
        NodePtr pnode = (NodePtr)malloc(sizeof(Node));
        if(!pnode){
            printf("Error: Ϊ���ݽ��%d�����ڴ�ʧ�ܣ�\n", R[i]);
            exit(0);
        }
        pnode->data = R[i];
        pnode->next = phead->next;
        phead->next = pnode;
    }
    return phead;
}
// ����ͷ���pheadָ��ĵ������ͷ��ڴ�ռ䣩
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

// ���Ժ�����˳�����������phead�е�Ԫ��ֵ������idx��ʾ������Ĭ��ֵΪ0��
void output_chain(NodePtr phead, int idx = 0)
{
    printf("**********************************************\n");
    printf("��%d�ַ���-�ռ������������\n", idx+1);
    printf("**********************************************\n");
    while(phead->next){
        printf("%d\n", phead->next->data);
        phead = phead->next;
    }
}
