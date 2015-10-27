/*********************************************
  *  P90 �㷨3.4  �������Ĳ�α����ݹ��㷨
**********************************************/

void level_order(BTreePtr pbt)
{
    BTreePtr Queue[MAXSIZE];
    int front = -1, rear = 0;
    
    if(pbt == NULL) return;    // �ն���������������
    
    Queue[rear] = pbt;
    
    while ( rear != front ){     // ��ջ�ǿ�ʱ
        front++;
        printf("%c ", Queue[front]->data);  // ���ʶ���Ԫ��
        
        if (Queue[front]->lchild != NULL) {   // �������ǿ�
            rear++;
            Queue[rear] = Queue[front]->lchild;
        }
        
        if (Queue[front]->rchild != NULL) {   // �������ǿ�
            rear++;
            Queue[rear] = Queue[front]->rchild;
        }
    }    
}
