/*************************************
 *  教材P56：队列的顺序存储结构（以循环队列为例）
**************************************/
# define MAXQUEUE 10
typedef int EntryType;
 
/*************************************
 * 教材P56：定义顺序队列的数据类型
**************************************/
typedef struct queue{
   int front, rear;                          // 队头和队尾指针
   EntryType entry[MAXQUEUE]; // 数据元素存储空间
}Queue, *QueuePtr;                    // 定义为新的数据类型

/*************************************
 * 教材P58：循环队列的初始化
**************************************/
int queue_init(QueuePtr *q){
    int status = 1;
    QueuePtr pQue = (QueuePtr)malloc(sizeof(Queue))
    if(pQue){
        pQue->front = pQue->rear = 0; // 取值范围：0～MAXQUEUE-1
        *q = pQue;
        status = 0;
    }
    return status;
}

/*************************************
 * 教材P58：循环队列的的销毁（释放队列的存储空间）
**************************************/
void queue_destroy(QueuePtr *q) {
	if(*q) {  // 首先判断队列的存储空间是否存在
		free (*q);
		*q = NULL;
	}
}

/*************************************
 * 教材P58：循环队列的清空（清除队列中存放的数据元素）
 * 队列中存放的有效数据由首尾指针指示，与队列空间中存储的具体值无关
**************************************/
void queue_clear(QueuePtr q){
	q->front = q->rear = 0; 
}

/*************************************
 * 教材P58：判空
 * 采用牺牲单位元素空间法，队列空：front == rear
**************************************/
bool queue_empty(QueuePtr q){
	return (q->front == q->rear);
}

/*************************************
 * 教材P58：判满
 * 采用牺牲单位元素空间法，队列满：(rear+1)%M==front
**************************************/
bool queue_full(QueuePtr q){
	return (q->rear + 1) % MAXQUEUE == q->front;
}

/*************************************
 * 教材P58：求队列元素个数
**************************************/
int queue_size(QueuePtr q){
	return (q->rear - q->front + MAXQUEUE) % MAXQUEUE ; 
}