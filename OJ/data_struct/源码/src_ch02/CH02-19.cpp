/*************************************
 *  �̲�P59��ѭ�����е�����㷨
**************************************/
int queue_append(QueuePtr q, EntryType item){
    int status = 1;
    if (! queue_full(q)){  // ������δ��
        q->entry[q->rear] = item;
        q->rear = (q->rear + 1) % MAXQUEUE;
        status = 0;
    }
    return status;
}
