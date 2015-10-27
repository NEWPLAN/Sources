// 例2：合并有序表（P20 例2.2）
// 已知线性表La和Lb中元素分别按非递减顺序排列
// 现要求将它们合并成一个新的线性表Lc
// 并使得Lc中元素也按照非递减顺序排列

typedef int ElemType;

int list_merge (ListPtr La, ListPtr Lb, ListPtr Lc)
{
    int m, n, status = 0;
    int i=1, j=1, k=1;    /*i, j, k分别用于指示La, Lb, Lc中当前元素*/
    ElemType elem1, elem2;

    status = list_init(Lc);

    n  = list_size(La);
    m = list_size(Lb);

    while(i<=n && j<=m)   // 两个表都还未处理完
    {
        list_retrieve(La, i, &elem1);
        list_retrieve(Lb, j, &elem2);
        if(elem1<elem2)
        {
            status = list_insert(Lc, k, elem1);
            i = i+1;
        }
        else
        {
            status = list_insert(Lc, k, elem2);
            j = j+1;
        }
        k = k+1;
    }
    while(i<=n)   // 表La还未处理完
    {
        list_retrieve(La, i, &elem1);
        status = list_insert(Lc, k, elem1);
        i = i+1;
        k = k+1;
    }
    while(j<=m)   // 表Lb还未处理完
    {
        list_retrieve(Lb, j, &elem2);
        status = list_insert(Lc, k, elem2);
        j = j+1;
        k = k+1;
    }
    return status;
}
