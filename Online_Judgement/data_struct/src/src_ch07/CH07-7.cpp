/*****************************************************************************
* 算法7.7  锦标赛排序算法（树状排序）
* 采用待排元素作为叶结点构造一棵完全二叉树，其深度为floor(logn)+1（其中 n 为待排序元素个数）。
* 首先对n个记录的关键字的取值进行两两比较，直到选出一个值最小的记录作为根结点（需要进行 n-1 次关键字比较）。
* 根据建成的二叉树依次选出后续的记录（每次选择所需进行的关键字比较次数均为 O(log2n)）。
* 因此，算法所需执行的关键字比较次数总数约为：O(nlog2n)。
* 为构造排序所需的二叉树，需增设额外的附加存储用于保存比较结果
******************************************************************************/ 

#define MAX (int)(((unsigned)(~((int)0)))>>1) 
#define MIN (-MAX-1) 

inline int get_min(int x, int y)
{
    return (x < y) ? x : y;
}

// 更新二叉树Tree[root ... end]的根节点（选出新的胜出记录）
void updata_root(int *Tree, int root, int end);
// 锦标赛排序算法
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

    // 注：Tree[0]保留（为了便于处理左右孩子）
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
        // Out(b, len); //不断跟踪输出完全二叉树b[]状态 
        updata_root( Tree, 1, len ); 
    }  
    free(Tree); 
} 


// 更新二叉树Tree[root ... end]的根节点（选出新的胜出记录）
void updata_root(int *Tree, int root, int end) 
{ 
    int lchild = root * 2; 
    int rchild = lchild + 1; 

    if ( lchild >= end ) {
        // 当下溯到叶节点时，将当前的“冠军”记录的值替换为MAX
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
