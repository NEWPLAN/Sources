/*********************************************
  *  P93 �㷨3.7  ͳ�ƶ�������Ҷ�ӽ���㷨
**********************************************/

int count_leafs(BTreePtr pbt)
{ 
    if( pbt== NULL ){
        return 0;
    }
    if((pbt->lchild == NULL) && (pbt->rchild == NULL)){
        return 1;
    }
    return(count_leaf_BT(pbt->lchild) + count_leaf_BT(pbt->rchild) );
}