// ��2���ϲ������P20 ��2.2��
// ��֪���Ա�La��Lb��Ԫ�طֱ𰴷ǵݼ�˳������
// ��Ҫ�����Ǻϲ���һ���µ����Ա�Lc
// ��ʹ��Lc��Ԫ��Ҳ���շǵݼ�˳������

typedef int ElemType;

int list_merge (ListPtr La, ListPtr Lb, ListPtr Lc)
{
    int m, n, status = 0;
    int i=1, j=1, k=1;    /*i, j, k�ֱ�����ָʾLa, Lb, Lc�е�ǰԪ��*/
    ElemType elem1, elem2;

    status = list_init(Lc);

    n  = list_size(La);
    m = list_size(Lb);

    while(i<=n && j<=m)   // ��������δ������
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
    while(i<=n)   // ��La��δ������
    {
        list_retrieve(La, i, &elem1);
        status = list_insert(Lc, k, elem1);
        i = i+1;
        k = k+1;
    }
    while(j<=m)   // ��Lb��δ������
    {
        list_retrieve(Lb, j, &elem2);
        status = list_insert(Lc, k, elem2);
        j = j+1;
        k = k+1;
    }
    return status;
}
