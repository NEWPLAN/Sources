/*************************************
 *  �̲�P59��ѭ�����еĳ����㷨
**************************************/
int queue_serve(QueuePtr q, EntryType *item){
    int status = 1;
    if (! queue_empty(q)){  // �����зǿ�
        *item = q->entry[q->front];
        q->front = (q->front + 1) % MAXQUEUE;
        status = 0;
    }
    return status;
}
