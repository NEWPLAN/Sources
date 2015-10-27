/*********************************************
  *  P92 �㷨3.6  �����������������н����������ĵݹ��㷨
**********************************************/

// �����������������н����������ĵݹ��㷨
// ����pre_seq��in_seq�ֱ���������������У�nΪ������������
BTreePtr createbt_preinorder (char * pre_seq, char * in_seq, int n)
{
    BTreePtr pbt;
    if(n <= 0){
        pbt = NULL;
    }
    else{
        preinorder(&pbt, pre_seq, in_seq, 0, n, 0, n);
    }
    return pbt;
}

// prea��preb�ֱ�ָʾ�������е���ֹλ�ã�ina��inb�ֱ�ָʾ�������е���ֹλ��
void preinorder( BTreePtr *ppbt, char * pre_seq, char * in_seq, 
                            int prea, int preb, int ina, int inb)
{
    int idx;
    (*ppbt) = (BTreePtr) malloc(sizeof(BTree));    // �������ĸ����
    (*ppbt)->lchild = NULL; 
    (*ppbt)->rchild = NULL;
    (*ppbt)->data = pre_seq[prea];   

    idx = ina;
    while(in_seq[idx] != pre_seq[prea]){
        idx++;                // �����������ж�λ�����
    }

    // �ݹ���ý���������
    if (idx == ina) {    // ������Ϊ��
        (*ppbt)->lchild = NULL;  
    }
    else{
        preinorder(&((*ppbt)->lchild), pre_seq, in_seq, (prea+1), (prea + idx - ina), ina, (idx-1));
    }

    // �ݹ���ý���������
    if (idx == inb) {    // ������Ϊ��
        (*ppbt)->rchild = NULL;  
    }
    else{
        preinorder(&((*ppbt)->rchild), pre_seq, in_seq, (prea + idx - ina + 1), preb, (idx+1), inb);
    }
}

