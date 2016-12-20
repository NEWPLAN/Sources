/***********************************************
 *  P72 ������Ԫ��ľ���ת���㷨 
************************************************/
#define    MAXSIZE    20

// ��Ԫ������ݽṹ
typedef   struct
{   int  rown, coln;  // Ԫ����ϡ������е��кź��к�
     DataType val;     // ����Ԫ��ֵ
}Triplet;

// ��Ԫ�����ϡ����󣩵����ݽṹ
Typedef struct{
     Triplet data[MAXSIZE+1];  // ����Ԫ��ΪԪ�ص�����
     int   nrows��ncols��nelem��   // ϡ�����������������ͷ���Ԫ����
}TSMatrix��


int trans_sparsemat(TSMatrix  *ma, TSMatrix *mb)
{  
    int col, i, k;

    // ����ϡ��������������������������Ԫ�ظ���
    mb->nrows = ma->ncols;
    mb->ncols = ma->nrows;
    mb->nelem = ma->nelem;

    if(ma->nelem > 0){
        k = 0;
        for( col = 0; col<(ma->ncols); col++ ){ // ��ma������ת��
            for( i = 0; i<(ma->nelem); i++ ){    // ɨ��������Ԫ���
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
