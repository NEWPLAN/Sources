/*********************************************
  *  P93 �㷨3.8  �������������㷨
**********************************************/

int get_depth(BTreePtr pbt)
{
    int depthL = 0, depthR = 0;   // ���ڽ��յ�ǰ�������������ȵķ���ֵ

    if( pbt== NULL ){
        return 0;                              // ���������Ϊ0
    }
    if((pbt->lchild == NULL) && (pbt->rchild == NULL)){
        return 1;                              // Ҷ��㣬���Ϊ1
    }

    depthL = get_depth_BT(pbt->lchild);    // ���������
    depthR = get_depth_BT(pbt->rchild);   // ���������

    // �����������Ϊ������������������ֵ�ϴ��߼�һ
    return 1 + ((depthL > depthR) ? depthL : depthR); 
}