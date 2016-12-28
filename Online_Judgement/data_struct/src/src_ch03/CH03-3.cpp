/*********************************************
  *  P92 算法3.6  由先序和中序遍历序列建立二叉树的递归算法
**********************************************/

// 由先序和中序遍历序列建立二叉树的递归算法
// 数组pre_seq和in_seq分别存放先序和中序序列，n为二叉树结点个数
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

// prea和preb分别指示先序序列的起止位置；ina和inb分别指示中序序列的起止位置
void preinorder( BTreePtr *ppbt, char * pre_seq, char * in_seq, 
                            int prea, int preb, int ina, int inb)
{
    int idx;
    (*ppbt) = (BTreePtr) malloc(sizeof(BTree));    // 二叉树的根结点
    (*ppbt)->lchild = NULL; 
    (*ppbt)->rchild = NULL;
    (*ppbt)->data = pre_seq[prea];   

    idx = ina;
    while(in_seq[idx] != pre_seq[prea]){
        idx++;                // 在中序序列中定位根结点
    }

    // 递归调用建立左子树
    if (idx == ina) {    // 左子树为空
        (*ppbt)->lchild = NULL;  
    }
    else{
        preinorder(&((*ppbt)->lchild), pre_seq, in_seq, (prea+1), (prea + idx - ina), ina, (idx-1));
    }

    // 递归调用建立右子树
    if (idx == inb) {    // 右子树为空
        (*ppbt)->rchild = NULL;  
    }
    else{
        preinorder(&((*ppbt)->rchild), pre_seq, in_seq, (prea + idx - ina + 1), preb, (idx+1), inb);
    }
}

