/***********************************************
 *  P72 基于三元组的矩阵转置算法 
************************************************/
#define    MAXSIZE    20

// 三元组的数据结构
typedef   struct
{   int  rown, coln;  // 元素在稀疏矩阵中的行号和列号
     DataType val;     // 矩阵元素值
}Triplet;

// 三元组表（即稀疏矩阵）的数据结构
Typedef struct{
     Triplet data[MAXSIZE+1];  // 以三元组为元素的数组
     int   nrows，ncols，nelem；   // 稀疏矩阵的行数、列数和非零元素数
}TSMatrix；


int trans_sparsemat(TSMatrix  *ma, TSMatrix *mb)
{  
    int col, i, k;

    // 交换稀疏矩阵的行数和列数，拷贝非零元素个数
    mb->nrows = ma->ncols;
    mb->ncols = ma->nrows;
    mb->nelem = ma->nelem;

    if(ma->nelem > 0){
        k = 0;
        for( col = 0; col<(ma->ncols); col++ ){ // 按ma的列序转换
            for( i = 0; i<(ma->nelem); i++ ){    // 扫描整个三元组表
                if(ma->data[i].coln == col){
                    mb->data[k].rown = ma->data[i].coln;
                    mb->data[k].coln = ma->data[i].rown;
                    mb->data[k].val = ma->data[i].val ;
                    k++;
                }
            }
        }// end of for( col = 0; col<(ma->ncols); col++ )
    }// end of if(ma->nelem > 0)
    return 0;
}
