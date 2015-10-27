/*********************************************
  *  P93 算法3.8  求二叉树的深度算法
**********************************************/

int get_depth(BTreePtr pbt)
{
    int depthL = 0, depthR = 0;   // 用于接收当前结点左右子树深度的返回值

    if( pbt== NULL ){
        return 0;                              // 空树，深度为0
    }
    if((pbt->lchild == NULL) && (pbt->rchild == NULL)){
        return 1;                              // 叶结点，深度为1
    }

    depthL = get_depth_BT(pbt->lchild);    // 左子树深度
    depthR = get_depth_BT(pbt->rchild);   // 右子树深度

    // 二叉树的深度为根结点左、右子树的深度值较大者加一
    return 1 + ((depthL > depthR) ? depthL : depthR); 
}