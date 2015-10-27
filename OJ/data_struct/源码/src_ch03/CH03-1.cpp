/*********************************************
  *  P90 算法3.4  二叉树的层次遍历递归算法
**********************************************/

void level_order(BTreePtr pbt)
{
    BTreePtr Queue[MAXSIZE];
    int front = -1, rear = 0;
    
    if(pbt == NULL) return;    // 空二叉树，遍历结束
    
    Queue[rear] = pbt;
    
    while ( rear != front ){     // 当栈非空时
        front++;
        printf("%c ", Queue[front]->data);  // 访问队首元素
        
        if (Queue[front]->lchild != NULL) {   // 左子树非空
            rear++;
            Queue[rear] = Queue[front]->lchild;
        }
        
        if (Queue[front]->rchild != NULL) {   // 右子树非空
            rear++;
            Queue[rear] = Queue[front]->rchild;
        }
    }    
}
