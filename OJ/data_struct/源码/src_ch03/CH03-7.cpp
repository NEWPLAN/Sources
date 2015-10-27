/*********************************************
  *  Huffman���Ĺ�����Huffman����
**********************************************/

// Huffman���Ľڵ�ṹ����
typedef struct huffman_tree_node
{
    int weight;
    int lchild, rchild, parent;
} HTNode, * HTPtr;

// Huffman������Ԫ�ؽṹ����
typedef struct huffman_code
{
    int weight;   // �����Ӧ��Ȩֵ
    char * pcode; // ָ������ַ�����ָ��
} HcodeNode, *HcodePtr;

// ����Huffman��
HTPtr create_huffman_tree(int weights[], int n );
// ��ɭ����ѡ��Ȩֵ��С������������ͨ������*s1��*s2��������ţ�
void select_nodes(HTPtr , int , int *, int *);

// ����Huffman���õ�Ҷ�ڵ��Huffman�����
HcodePtr get_hcodetab( HTPtr , int  );
// ����Huffman�������ַ������б���(��ʾ��)�����ر��봮
char * encode_huffman(HcodePtr, char *, char *, int);
// ����Huffman�����Ա��봮���н���(��ʾ��)�����������ַ���
char * decode_huffman(HTPtr, char *, char *, int);

// ����Huffman������ͷ��ڴ�ռ䣩
void destroy_table(HcodePtr hcode_table, int n);


// ����Huffman��
HTPtr create_huffman_tree( int weights[], int n )
{
    HTPtr pht;
    int s1, s2;          // ���ڱ���Ȩֵ��С���������������
    int i, n_nodes = 0;

    if( n < 1 ) return NULL;
    n_nodes = (2 * n) - 1;  // Huffman���Ľڵ���Ϊ2n-1

    pht = ( HTNode * )malloc( sizeof( HTNode ) * n_nodes );
    if( pht == NULL )
    {
        exit(0);
    }

    // Huffman�����ʼ�������������ڴ��Huffman����������в��������������ĸ��ڵ���ţ�
    for( i = 0; i < n_nodes; i++ )
    {
        pht[i].weight = (i < n) ? weights[i] : 0;
        pht[i].lchild = -1;
        pht[i].rchild = -1;
        pht[i].parent = -1;
    }

    for( i = n; i < n_nodes; ++i )
    {
        // �ӵ�ǰ������ѡ������Ȩֵ��С�Ҹ�ָ��Ϊ-1�Ľ�㣨�������ĸ���㣩
        select_nodes( pht, (i-1), &s1, &s2 );

        // �ϲ�����s1��s2���ϲ���Ľ�������λ��i�������������Ϊi��
        pht[s1].parent = pht[s2].parent = i;

        pht[i].lchild = s1;
        pht[i].rchild = s2;
        pht[i].weight = pht[s1].weight + pht[s2].weight;
    }

    return pht;
}

// ����select_nodes�Ĺ���Ϊѡ��������Ȩֵ��С������������ָ��Ϊ-1��
// ѡ�����������ڵ����ͨ��ָ�����s1��s2�����޸�
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

// ���Huffman���ṹ
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

// Huffman����: ��ÿ��Ҷ�ڵ����ݣ��õ������0-1����
// Լ�������֧����Ϊ'0'���ҷ�֧����Ϊ'1'
// ����nΪHuffman����Ҷ�ڵ���������Ҷ�ڵ��Huffman�����HcodeNode���͵����飩
HcodePtr get_hcodetab( HTPtr pht, int n )
{
    int i, idx, start, parent_id, size_code;
    HcodePtr hcode_table;  // ָ��HcodeNode���������ָ��

    // ����һ��������ַ�������ʱ�����ռ�
    // n��Ҷ�ڵ㣬������Ҫnλ0-1�ַ����룬�����һ���ַ��Ŀռ����ڴ洢�ַ�����β��'\0'
    char * pch = ( char * )malloc( sizeof( char ) * (n+1) );
    if( !pch )   // assert( pch != NULL );
    {
        printf("Error: Ϊ�����ַ������빤���ռ�ʧ�ܣ�");
        exit(0);
    }
    memset( pch, 0, sizeof( char) * (n+1) ); // ���·���Ŀռ�ִ���������

    // ΪHuffman��������飩���ٿռ䣨���ڴ��n��Ҷ�ڵ��Huffman����Ͷ�ӦȨֵ��
    // ����Ԫ������ΪHcodeNode�ṹ������
    // ����Ԫ�صĳ�Ա��weight��Ž��Ȩֵ��pcode��ű����ַ���ָ��
    hcode_table = ( HcodePtr )malloc( sizeof( HcodeNode ) * n );
    if( !hcode_table )   // assert( hcode_table != NULL );
    {
        printf("Error: ΪHuffman����������ڴ�ռ�ʧ�ܣ�");
        exit(0);
    }

    // ��Huffman����ȡҶ�ڵ�Ȩֵ����ֵ��Huffman������ӦԪ��
    for( i = 0; i < n; ++i )
    {
        hcode_table[i].weight = pht[i].weight;
    }

    // ��n��Ҷ�ڵ���һ����Huffman��(���ݵ������)�õ���Huffman����
    for( i = 0; i < n; ++i )
    {
        start = n; // ָʾ�������е���ʼλ�ã�nΪpch�������һ��Ԫ�ص��±꣬��ʾ����������䣩
        idx = i;
        parent_id = pht[idx].parent;

        while( parent_id!= -1 )     // �����׽�㲻Ϊ��1������Ϊ�����ʱ��
        {
            //pch[--pos] = (pht[parent_id].lchild == tmp )? '0' : '1'; // ����if-else���ļ�д��
            if (pht[parent_id].lchild == idx)
            {
                pch[--start] = '0';    // ���֧����Ϊ'0'
            }
            else
            {
                pch[--start] = '1';    // �ҷ�֧����Ϊ'1'
            }
            idx = parent_id;
            parent_id = pht[idx].parent;
        }
        // (n - start)ΪҶ�ڵ�i��Huffman���볤�ȣ�+1��Ŀ���ǰ����ַ�����β��'\0'�ַ�
        size_code =  n - start + 1;

        // ΪHuffman�������Ԫ�ص�pcode���ַ�����ָ�룩��Ա����洢�ռ䣨���ڴ�ű��봮��
        hcode_table[i].pcode = ( char * )malloc( sizeof( char ) * size_code ); // Ϊ���Ĵ�����ռ�
        if( !hcode_table[i].pcode )   // assert( hcode_table[i].pcode != NULL );
        {
            printf("Error: Ϊ�ڵ�%d��Huffman���������ڴ�ռ�ʧ�ܣ�", i);
            exit(0);
        }
        // ��&pch[start]ָ����ַ����������·�����ַ�����ռ���
        strcpy_s( hcode_table[i].pcode, size_code, &pch[start] );
    }

    free(pch);  // ������ʱ�����ռ�
    return hcode_table;
}

// ����Huffman�������ַ������б���(��ʾ��)�����ر��봮
// ����strΪ�����ַ���������dictΪ�ֵ�
char * encode_huffman(HcodePtr hcode_table, char * str, char * dict, int nleaves)
{
    int i, j, k, codelen, idx = 0;
    int len = strlen(str);
    int wpl = 1000;  // �����滻Ϊ��WPL�ĺ���
    char * pch = ( char * )malloc( sizeof( char ) * (wpl+1) );
    if( !pch )   // assert( pch != NULL );
    {
        printf("Error: Ϊ���봮���빤���ռ�ʧ�ܣ�");
        exit(0);
    }
    memset( pch, 0, sizeof( char) * (wpl+1) ); // ���·���Ŀռ�ִ���������

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


// ����Huffman�����Ա��봮���н���(��ʾ��)�����������ַ���
// ����codeΪ���봮������dictΪ�ֵ�
char * decode_huffman(HTPtr pht, char * code, char * dict, int nleaves)
{
    int i, pos = 0, idx = 0;
    int len = strlen(code);
    int total = 1000;  // �����滻Ϊ��Ȩ�غ͵ĺ���
    char * pch = ( char * )malloc( sizeof( char ) * (total+1) );
    if( !pch )   // assert( pch != NULL );
    {
        printf("Error: Ϊ���봮���빤���ռ�ʧ�ܣ�");
        exit(0);
    }
    memset( pch, 0, sizeof( char) * (total+1) ); // ���·���Ŀռ�ִ���������

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


// ����Huffman�����
void destroy_table(HcodePtr hcode_table, int n)
{
    int i;
    for(i = 0; i < n; ++i)
    {
        // ��Huffman��������飩��ÿ��Ԫ�أ�����Ϊ�����ı��루�ַ�������ռ�
        free(hcode_table[i].pcode);
    }
    // ����Huffman��������飩�ռ�
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

    // �õ�Huffman����Ҷ�ڵ���
    nleaves = sizeof(weights)/ sizeof(weights[0]);
    // ����Huffman��
    pht = create_huffman_tree( weights, nleaves );
    // ���Huffman���Ľṹ
    output_huffman_tree(pht, nleaves);
    // ����Huffman�����
    hcode_table = get_hcodetab( pht, nleaves );
    // ���Huffman����Ҷ�ڵ�Ȩ�ؼ���Huffman����
    for(i = 0; i < nleaves ; i++)
    {
        printf("%2c:%3d:%6s\n", characters[i], hcode_table[i].weight, hcode_table[i].pcode);
    }

    // ��"this is a good boy"���б���
    pcode = encode_huffman(hcode_table, mystr, characters, nleaves);
    printf("%s\n", pcode);

    // ��pcode���н���
    ptxt = decode_huffman(pht, pcode, characters, nleaves);
    printf("%s\n", ptxt);

    free(pcode);
    free(ptxt);

    // ����Huffman�����
    destroy_table( hcode_table,  nleaves);

    // ����ΪHuffman��������ڴ�ռ�
    free(pht);

    system("pause");
    return 0;
}
