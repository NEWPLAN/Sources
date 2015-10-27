/*****************************************************************************
* �㷨7.7  �����������㷨����״����
* ���ô���Ԫ����ΪҶ��㹹��һ����ȫ�������������Ϊfloor(logn)+1������ n Ϊ������Ԫ�ظ�������
* ���ȶ�n����¼�Ĺؼ��ֵ�ȡֵ���������Ƚϣ�ֱ��ѡ��һ��ֵ��С�ļ�¼��Ϊ����㣨��Ҫ���� n-1 �ιؼ��ֱȽϣ���
* ���ݽ��ɵĶ���������ѡ�������ļ�¼��ÿ��ѡ��������еĹؼ��ֱȽϴ�����Ϊ O(log2n)����
* ��ˣ��㷨����ִ�еĹؼ��ֱȽϴ�������ԼΪ��O(nlog2n)��
* Ϊ������������Ķ����������������ĸ��Ӵ洢���ڱ���ȽϽ��
******************************************************************************/ 

#define MAX (int)(((unsigned)(~((int)0)))>>1) 
#define MIN (-MAX-1) 

inline int get_min(int x, int y)
{
    return (x < y) ? x : y;
}

// ���¶�����Tree[root ... end]�ĸ��ڵ㣨ѡ���µ�ʤ����¼��
void updata_root(int *Tree, int root, int end);
// �����������㷨
void sort_tournament( int R[], int num ) ;

int main() 
{ 
    int i, num; 
    int keys[] = { 21, 25, 49, 25, 16, 8, 63, 63, 100, 1002 }; 
    num = sizeof(keys)/sizeof(keys[0]);

    sort_tournament(keys, num); 

    for(i = 0; i < num; ++i){
        printf("%2d\n", keys[i]);        
    }
    system("pause");
    return 0;
} 

void sort_tournament( int R[], int num ) 
{ 
    int i, len, idx, *Tree; 
    //void Out(int *, int); 

    len = 1; 
    while (len < num ) { 
        len <<= 1; 
    } 
    len = 2 * len; 
    //printf("%d", len);

    // ע��Tree[0]������Ϊ�˱��ڴ������Һ��ӣ�
    Tree = (int *)malloc(sizeof(int) * len); 

    for ( i = (len / 2); i < len; ++i ) { 
        idx = i - len / 2;
        Tree[i] = ( idx < num ) ? R[idx] : MAX; 
        //printf("%d:%d\n", i, Tree[i]);
    }  
    for (i = (len / 2)-1 ; i > 0; --i) { 
        Tree[i] = get_min(Tree[2 * i ], Tree[2 * i + 1]); 
        //printf("%d:%d\n", i, Tree[i]);
    } 

    for (i = 0; i < num; i++) { 
        R[i] = Tree[1]; 
        // Out(b, len); //���ϸ��������ȫ������b[]״̬ 
        updata_root( Tree, 1, len ); 
    }  
    free(Tree); 
} 


// ���¶�����Tree[root ... end]�ĸ��ڵ㣨ѡ���µ�ʤ����¼��
void updata_root(int *Tree, int root, int end) 
{ 
    int lchild = root * 2; 
    int rchild = lchild + 1; 

    if ( lchild >= end ) {
        // �����ݵ�Ҷ�ڵ�ʱ������ǰ�ġ��ھ�����¼��ֵ�滻ΪMAX
        Tree[root] = MAX; 
        return; 
    } 
    if ( Tree[lchild] == Tree[root] ) { 
        updata_root(Tree, lchild, end); 
    } 
    else { 
        updata_root(Tree, rchild, end); 
    } 
    Tree[root] = get_min(Tree[lchild], Tree[rchild]); 
} 
