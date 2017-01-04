/*************************************
 *  教材P59：循环队列的入队算法
**************************************/
int queue_append(QueuePtr q, EntryType item){
    int status = 1;
    if (! queue_full(q)){  // 若队列未满
        q->entry[q->rear] = item;
        q->rear = (q->rear + 1) % MAXQUEUE;
        status = 0;
    }
    return status;
}
