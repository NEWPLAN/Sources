/*********************************************
  *  P95 算法3.9  二叉树的中序线索化
**********************************************/

// 中序遍历二叉树pbt，并对其进行线索化
void inorder_threading(BTreePtr pbt, BTreePtr *prev)
{
    if (pbt) {
        inorder_threading(pbt->lchild, prev);  // 对左子树进行中序线索化
        
        // 若pbt无左孩子，则将当前结点的左孩子指针用作线索
        // 令当前结点的左孩子指针指向前驱结点*prev
        if(pbt->lchild == NULL){
            pbt->ltag = 1;
            pbt->lchild = (*prev);
        }
        
        // 若prev无右孩子，则将prev结点的右孩子指针用作线索
        // 令前驱结点*prev的后继线索(右孩子指针)指向当前结点
        if(prev->rchild == NULL){
            prev->rtag = 1;
            (*prev)->rchild = pbt;
        }
        
        // 修改前驱结点指针，令其指向当前结点
        *prev = pbt;
        
        inorder_threading(pbt->rchild, prev);  // 对右子树进行中序线索化
    }
}

// 根据已有二叉树pbt生成一棵带头结点的中序线索二叉树
BTreePtr mk_thread_btree(BTreePtr pbt)
{
    BTreePtr head;
    BTreePtr *prev;
    
    head =  (BTreePtr)malloc(sizeof(BTNode));
    if ( !head ){
        printf("Error: 内存耗尽，线索树生成失败");
        exit(0);
    }
    
    // 头结点初始化（根据P94对头结点的定义设定）
    head->ltag = 0;           // 根据定义，头结点的左孩子指向树根结点 
    head->lchild = head;   // 令左指针回指头结点自身（以防二叉树为空树） 
    
    head->rtag = 1;           // 头结点的右孩子为线索，指向中序遍历的最后一个结点
    head->rchild = head;   // 令右指针回指头结点自身（目前还不知道尾结点的位置）
    
    prev = &head;    // 令前驱指针指向线索树头结点
    
    if( pbt ){                                       // 如果二叉树非空
        inorder_threading(pbt, prev);   // 对其执行中序线索化
    }
    
    // 中序遍历结束，修改头结点的右指针域，令其指向中序遍历的最后一个结点
    head->rchild = (*prev);
    
    // 修改中序遍历序列尾结点的右指针域，令其指向头结点，同时将线索标志置为1
    (*prev)->rtag = 1; 
    (*prev)->rchild = head;

	return head;
}