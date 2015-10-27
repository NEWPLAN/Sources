/*********************************************
  *  Huffman树的构造与Huffman编码
**********************************************/

// Huffman树的节点结构定义
typedef struct huffman_tree_node
{
    int weight;
    int lchild, rchild, parent;
} HTNode, * HTPtr;

// Huffman编码表的元素结构定义
typedef struct huffman_code
{
    int weight;   // 编码对应的权值
    char * pcode; // 指向编码字符串的指针
} HcodeNode, *HcodePtr;

// 构造Huffman树
HTPtr create_huffman_tree(int weights[], int n );
// 从森林中选择权值最小的两棵子树（通过参数*s1和*s2返回其序号）
void select_nodes(HTPtr , int , int *, int *);

// 根据Huffman树得到叶节点的Huffman编码表
HcodePtr get_hcodetab( HTPtr , int  );
// 根据Huffman编码表对字符串进行编码(简单示例)，返回编码串
char * encode_huffman(HcodePtr, char *, char *, int);
// 根据Huffman编码表对编码串进行解码(简单示例)，返回明文字符串
char * decode_huffman(HTPtr, char *, char *, int);

// 销毁Huffman编码表（释放内存空间）
void destroy_table(HcodePtr hcode_table, int n);


// 构造Huffman树
HTPtr create_huffman_tree( int weights[], int n )
{
    HTPtr pht;
    int s1, s2;          // 用于保存权值最小的两棵子树的序号
    int i, n_nodes = 0;

    if( n < 1 ) return NULL;
    n_nodes = (2 * n) - 1;  // Huffman树的节点数为2n-1

    pht = ( HTNode * )malloc( sizeof( HTNode ) * n_nodes );
    if( pht == NULL )
    {
        exit(0);
    }

    // Huffman数组初始化（该数组用于存放Huffman树构造过程中产生的所有子树的根节点序号）
    for( i = 0; i < n_nodes; i++ )
    {
        pht[i].weight = (i < n) ? weights[i] : 0;
        pht[i].lchild = -1;
        pht[i].rchild = -1;
        pht[i].parent = -1;
    }

    for( i = n; i < n_nodes; ++i )
    {
        // 从当前数组中选择两个权值最小且父指针为-1的结点（即子树的根结点）
        select_nodes( pht, (i-1), &s1, &s2 );

        // 合并子树s1和s2，合并后的结果存放于位置i（树根结点的序号为i）
        pht[s1].parent = pht[s2].parent = i;

        pht[i].lchild = s1;
        pht[i].rchild = s2;
        pht[i].weight = pht[s1].weight + pht[s2].weight;
    }

    return pht;
}

// 函数select_nodes的功能为选出数组中权值最小两棵子树（父指针为-1）
// 选出的子树根节点序号通过指针参数s1和s2进行修改
void select_nodes(HTPtr pht, int n, int *s1, int *s2)
{
    int wa, wb, id, idxa = -1, idxb = -1;
    wa = wb = 0xFFFFFFF;

    for(id = 0; id <= n; id++)
    {
        if(pht[id].parent == -1)
        {
            if( pht[id].weight < wa )
            {
                idxb = idxa;
                idxa = id;
                wa = pht[id].weight;
            }
            else if(pht[id].weight < wb )
            {
                idxb = id;
                wb = pht[id].weight;
            }
        }
    }
    *s1 = idxa;
    *s2 = idxb;
    return;
}

// 输出Huffman树结构
void output_huffman_tree(HTPtr pht, int n)
{
    int n_nodes, i = 0;
    n_nodes = (2 * n) - 1;
    printf("location\tparent\tleft\tright\tweight\n");
    for(i = 0; i < n_nodes ; i++)
    {
        printf("%8d\t%6d\t%4d\t%5d\t%6d\n", i, pht[i].parent,
               pht[i].lchild, pht[i].rchild, pht[i].weight);
    }
}

// Huffman编码: 从每个叶节点上溯，得到编码的0-1序列
// 约定：左分支编码为'0'，右分支编码为'1'
// 参数n为Huffman树的叶节点数，返回叶节点的Huffman编码表（HcodeNode类型的数组）
HcodePtr get_hcodetab( HTPtr pht, int n )
{
    int i, idx, start, parent_id, size_code;
    HcodePtr hcode_table;  // 指向HcodeNode类型数组的指针

    // 开辟一个求编码字符串的临时工作空间
    // n个叶节点，至多需要n位0-1字符编码，多分配一个字符的空间用于存储字符串结尾的'\0'
    char * pch = ( char * )malloc( sizeof( char ) * (n+1) );
    if( !pch )   // assert( pch != NULL );
    {
        printf("Error: 为编码字符串申请工作空间失败！");
        exit(0);
    }
    memset( pch, 0, sizeof( char) * (n+1) ); // 对新分配的空间执行清零操作

    // 为Huffman编码表（数组）开辟空间（用于存放n个叶节点的Huffman编码和对应权值）
    // 数组元素类型为HcodeNode结构体类型
    // 数组元素的成员：weight存放结点权值；pcode存放编码字符串指针
    hcode_table = ( HcodePtr )malloc( sizeof( HcodeNode ) * n );
    if( !hcode_table )   // assert( hcode_table != NULL );
    {
        printf("Error: 为Huffman编码表申请内存空间失败！");
        exit(0);
    }

    // 从Huffman树读取叶节点权值，赋值给Huffman编码表对应元素
    for( i = 0; i < n; ++i )
    {
        hcode_table[i].weight = pht[i].weight;
    }

    // 对n个叶节点逐一查找Huffman树(上溯到根结点)得到其Huffman编码
    for( i = 0; i < n; ++i )
    {
        start = n; // 指示编码序列的起始位置（n为pch数组最后一个元素的下标，表示从右向左填充）
        idx = i;
        parent_id = pht[idx].parent;

        while( parent_id!= -1 )     // 当父亲结点不为－1（即不为根结点时）
        {
            //pch[--pos] = (pht[parent_id].lchild == tmp )? '0' : '1'; // 下述if-else语句的简化写法
            if (pht[parent_id].lchild == idx)
            {
                pch[--start] = '0';    // 左分支编码为'0'
            }
            else
            {
                pch[--start] = '1';    // 右分支编码为'1'
            }
            idx = parent_id;
            parent_id = pht[idx].parent;
        }
        // (n - start)为叶节点i的Huffman编码长度，+1的目的是包含字符串结尾的'\0'字符
        size_code =  n - start + 1;

        // 为Huffman编码表中元素的pcode（字符数组指针）成员分配存储空间（用于存放编码串）
        hcode_table[i].pcode = ( char * )malloc( sizeof( char ) * size_code ); // 为密文串分配空间
        if( !hcode_table[i].pcode )   // assert( hcode_table[i].pcode != NULL );
        {
            printf("Error: 为节点%d的Huffman编码申请内存空间失败！", i);
            exit(0);
        }
        // 将&pch[start]指向的字符串拷贝到新分配的字符数组空间中
        strcpy_s( hcode_table[i].pcode, size_code, &pch[start] );
    }

    free(pch);  // 销毁临时工作空间
    return hcode_table;
}

// 根据Huffman编码表对字符串进行编码(简单示例)，返回编码串
// 参数str为明文字符串，参数dict为字典
char * encode_huffman(HcodePtr hcode_table, char * str, char * dict, int nleaves)
{
    int i, j, k, codelen, idx = 0;
    int len = strlen(str);
    int wpl = 1000;  // 可以替换为求WPL的函数
    char * pch = ( char * )malloc( sizeof( char ) * (wpl+1) );
    if( !pch )   // assert( pch != NULL );
    {
        printf("Error: 为编码串申请工作空间失败！");
        exit(0);
    }
    memset( pch, 0, sizeof( char) * (wpl+1) ); // 对新分配的空间执行清零操作

    for(i = 0; i < len; ++i)
    {
        for(j = 0; j < nleaves; ++j)
        {
            if(str[i] == dict[j])
            {
                codelen = strlen(hcode_table[j].pcode);
                for(k = 0; k < codelen; ++k)
                {
                    pch[idx++] = hcode_table[j].pcode[k];
                }
                break;
            }
        }
    }
    return pch;
}


// 根据Huffman编码表对编码串进行解码(简单示例)，返回明文字符串
// 参数code为编码串，参数dict为字典
char * decode_huffman(HTPtr pht, char * code, char * dict, int nleaves)
{
    int i, pos = 0, idx = 0;
    int len = strlen(code);
    int total = 1000;  // 可以替换为求权重和的函数
    char * pch = ( char * )malloc( sizeof( char ) * (total+1) );
    if( !pch )   // assert( pch != NULL );
    {
        printf("Error: 为编码串申请工作空间失败！");
        exit(0);
    }
    memset( pch, 0, sizeof( char) * (total+1) ); // 对新分配的空间执行清零操作

    for(i = 0; i < len; )
    {
        idx = (2 * nleaves) - 2;
        while ( idx >= nleaves)
        {
            if( code[i] == '0')
            {
                idx = pht[idx].lchild;
                ++i;
            }
            else
            {
                idx = pht[idx].rchild;
                ++i;
            }
        }
        pch[pos++] = dict[idx];
    }
    return pch;
}


// 销毁Huffman编码表
void destroy_table(HcodePtr hcode_table, int n)
{
    int i;
    for(i = 0; i < n; ++i)
    {
        // 对Huffman编码表（数组）的每个元素，回收为其分配的编码（字符）数组空间
        free(hcode_table[i].pcode);
    }
    // 回收Huffman编码表（数组）空间
    free(hcode_table);
    return;
}

int main(void)
{
    HTPtr pht;
    HcodePtr hcode_table;
    //int weights[] = {5, 29, 7, 8, 14, 23, 3, 11};

    //int weights[] = {6, 7, 9, 5, 2};
    //int weights[] = {2, 4, 2, 3, 3};

    char * mystr = "this is a good boy";
    char characters[]= {'t', 'h','i','s','a','g','o','d','b','y',' '};
    int weights[] =   {  1,   1,  2,  2,  1,  1,  3,  1,  1,  1, 4 };
    char * pcode;
    char * ptxt;

    int i;
    int nleaves = 0;

    // 得到Huffman树的叶节点数
    nleaves = sizeof(weights)/ sizeof(weights[0]);
    // 创建Huffman树
    pht = create_huffman_tree( weights, nleaves );
    // 输出Huffman树的结构
    output_huffman_tree(pht, nleaves);
    // 构造Huffman编码表
    hcode_table = get_hcodetab( pht, nleaves );
    // 输出Huffman树的叶节点权重及其Huffman编码
    for(i = 0; i < nleaves ; i++)
    {
        printf("%2c:%3d:%6s\n", characters[i], hcode_table[i].weight, hcode_table[i].pcode);
    }

    // 对"this is a good boy"进行编码
    pcode = encode_huffman(hcode_table, mystr, characters, nleaves);
    printf("%s\n", pcode);

    // 对pcode进行解码
    ptxt = decode_huffman(pht, pcode, characters, nleaves);
    printf("%s\n", ptxt);

    free(pcode);
    free(ptxt);

    // 销毁Huffman编码表
    destroy_table( hcode_table,  nleaves);

    // 回收为Huffman树分配的内存空间
    free(pht);

    system("pause");
    return 0;
}
