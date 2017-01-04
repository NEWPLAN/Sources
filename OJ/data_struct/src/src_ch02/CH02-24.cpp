/***********************************************
 *  P73 ������Ԫ��ľ������ת���㷨 
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

// ma�Ǿ������Ԫ����ú���ʵ�ֶ�maת�õ���⣬���������mb��
int fast_trans_sparsemat(TSMatrix  *ma, TSMatrix *mb)
{  
    int col, i, k, pos;
    int num[ma->ncols];    // ��ž���ma�����еķ���Ԫ����
    int offset[ma->ncols];  // ָʾma�и��е�һ������Ԫ��mb�е�λ��
    
    // ����ϡ��������������������������Ԫ�ظ���
    mb->nrows = ma->ncols;
    mb->ncols = ma->nrows;
    mb->nelem = ma->nelem;
    
    if(ma->nelem > 0){
        // ��num���г�ʼ��
        for(col=0; col<ma->ncols; col++){
            num[col] = 0;  
        }
    
        // ����ma��ÿһ�з���Ԫ�صĸ���
        for(i=0; i<ma->nelem; i++){
            k = ma->data[i].coln;
            num[k]++;
        }
    
        offset[0] = 0;  // ��ʾma�е�һ�е�һ������Ԫ����mb->data�е�λ���±꣨ƫ������Ϊ0
        // ����ma��ÿһ�е�һ������Ԫ����mb->data�е�λ��
        for(i=1; i<ma->ncols; i++){
            offset[i] = offset[i-1] + num[i-1];
        }
        
        for( i = 0; i<(ma->nelem); i++ ){    // ɨ��������Ԫ���
            k = ma->data[i].coln;                    // ȡ�õ�ǰ��Ԫ���Ӧ��Ԫ�ص��к� 
            pos = offset[k];                              // ��ǰ��Ԫ����mb->data��Ӧ����λ��
            
            mb->data[pos].rown = ma->data[i].coln;
            mb->data[pos].coln = ma->data[i].rown;
            mb->data[pos].val = ma->data[i].val ;
            
            offset[k]++;              // ����ƫ����
            }
        }// end of for( i = 0; i<(ma->nelem); i++ )
    }// end of if(ma->nelem > 0)
    return 0;
}
