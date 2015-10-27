/*********************************************
  *  按先序遍历序列建立二叉树的二叉链表
  *  已知先序序列：A B C 0 0 D E 0 G 0 0 F 0 0 0
**********************************************/
BTreePtr create_prebt(BTreePtr pbt, char * pre_seq, int *pid)
{
    char ch;
    int idx = *pid;    

    if (idx <= (int)(strlen(pre_seq))){
        ch = pre_seq[idx];
        if(ch=='0'){
            (*pid)++;
            pbt = NULL;
        }
        else {
            pbt = (BTreePtr) malloc(sizeof(BTNode));        
            pbt->lchild = NULL;
            pbt->rchild = NULL;
            pbt->data = ch;
            (*pid)++;

            pbt->lchild = create_prebt(pbt->lchild, pre_seq, pid);
            pbt->rchild = create_prebt(pbt->rchild, pre_seq, pid );    
        }
    }
    return(pbt);
}

int main(void)
{ 
    int idx = 0;
    char pre_seq[] = "ABC00DE0G00F000"; 
    BTreePtr root = NULL;
    root = create_prebt(root, pre_seq, &idx);
    return 0;
}