/*********************************************
  *  P93 算法3.7  统计二叉树的叶子结点算法
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