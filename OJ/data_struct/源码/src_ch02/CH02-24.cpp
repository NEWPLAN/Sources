/***********************************************
 *  P73 基于三元组的矩阵快速转置算法 
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

// ma是矩阵的三元组表，该函数实现对ma转置的求解，结果保存在mb中
int fast_trans_sparsemat(TSMatrix  *ma, TSMatrix *mb)
{  
    int col, i, k, pos;
    int num[ma->ncols];    // 存放矩阵ma各列中的非零元个数
    int offset[ma->ncols];  // 指示ma中各列第一个非零元在mb中的位置
    
    // 交换稀疏矩阵的行数和列数，拷贝非零元素个数
    mb->nrows = ma->ncols;
    mb->ncols = ma->nrows;
    mb->nelem = ma->nelem;
    
    if(ma->nelem > 0){
        // 对num进行初始化
        for(col=0; col<ma->ncols; col++){
            num[col] = 0;  
        }
    
        // 计算ma中每一列非零元素的个数
        for(i=0; i<ma->nelem; i++){
            k = ma->data[i].coln;
            num[k]++;
        }
    
        offset[0] = 0;  // 表示ma中第一列第一个非零元素在mb->data中的位置下标（偏移量）为0
        // 计算ma中每一列第一个非零元素在mb->data中的位置
        for(i=1; i<ma->ncols; i++){
            offset[i] = offset[i-1] + num[i-1];
        }
        
        for( i = 0; i<(ma->nelem); i++ ){    // 扫描整个三元组表
            k = ma->data[i].coln;                    // 取得当前三元组对应的元素的列号 
            pos = offset[k];                              // 当前三元组在mb->data中应处的位置
            
            mb->data[pos].rown = ma->data[i].coln;
            mb->data[pos].coln = ma->data[i].rown;
            mb->data[pos].val = ma->data[i].val ;
            
            offset[k]++;              // 更新偏移量
            }
        }// end of for( i = 0; i<(ma->nelem); i++ )
    }// end of if(ma->nelem > 0)
    return 0;
}
