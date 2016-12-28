/*************************************
 *  教材P59：循环队列的出队算法
**************************************/
int queue_serve(QueuePtr q, EntryType *item){
    int status = 1;
    if (! queue_empty(q)){  // 若队列非空
        *item = q->entry[q->front];
        q->front = (q->front + 1) % MAXQUEUE;
        status = 0;
    }
    return status;
}
