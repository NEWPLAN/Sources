/*********************************************
  *  P95 �㷨3.9  ������������������
**********************************************/

// �������������pbt�����������������
void inorder_threading(BTreePtr pbt, BTreePtr *prev)
{
    if (pbt) {
        inorder_threading(pbt->lchild, prev);  // ����������������������
        
        // ��pbt�����ӣ��򽫵�ǰ��������ָ����������
        // �ǰ��������ָ��ָ��ǰ�����*prev
        if(pbt->lchild == NULL){
            pbt->ltag = 1;
            pbt->lchild = (*prev);
        }
        
        // ��prev���Һ��ӣ���prev�����Һ���ָ����������
        // ��ǰ�����*prev�ĺ������(�Һ���ָ��)ָ��ǰ���
        if(prev->rchild == NULL){
            prev->rtag = 1;
            (*prev)->rchild = pbt;
        }
        
        // �޸�ǰ�����ָ�룬����ָ��ǰ���
        *prev = pbt;
        
        inorder_threading(pbt->rchild, prev);  // ����������������������
    }
}

// �������ж�����pbt����һ�ô�ͷ������������������
BTreePtr mk_thread_btree(BTreePtr pbt)
{
    BTreePtr head;
    BTreePtr *prev;
    
    head =  (BTreePtr)malloc(sizeof(BTNode));
    if ( !head ){
        printf("Error: �ڴ�ľ�������������ʧ��");
        exit(0);
    }
    
    // ͷ����ʼ��������P94��ͷ���Ķ����趨��
    head->ltag = 0;           // ���ݶ��壬ͷ��������ָ��������� 
    head->lchild = head;   // ����ָ���ָͷ��������Է�������Ϊ������ 
    
    head->rtag = 1;           // ͷ�����Һ���Ϊ������ָ��������������һ�����
    head->rchild = head;   // ����ָ���ָͷ�������Ŀǰ����֪��β����λ�ã�
    
    prev = &head;    // ��ǰ��ָ��ָ��������ͷ���
    
    if( pbt ){                                       // ����������ǿ�
        inorder_threading(pbt, prev);   // ����ִ������������
    }
    
    // ��������������޸�ͷ������ָ��������ָ��������������һ�����
    head->rchild = (*prev);
    
    // �޸������������β������ָ��������ָ��ͷ��㣬ͬʱ��������־��Ϊ1
    (*prev)->rtag = 1; 
    (*prev)->rchild = head;

	return head;
}