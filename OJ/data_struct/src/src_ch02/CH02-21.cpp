/*************************************
 *  集合划分子集算法
**************************************/
// 注：conf和result维度为n；cirQue维度为n+1（即牺牲一个元素）

void  division( int R[][N], int cirQue[],
                        int conf[], int result[],  int n)
{
    int k, item, prev, groupNum;
    
    // 循环队列初始化
    for(k=0; k<n; k++) {
        cirQue[k] = k+1; 
    }
        
    front = 0;   // 队首指针指向最早入队的元素位置
    rear = n;    // 队尾指针指向向下一个可以存储数据的位置
    prev = rear;          // 记录当前的队尾位置
    groupNum = 0;     // 分组编号（从1开始）
    
    // 冲突矩阵初始化
    for(k=0; k<n; k++){
        conf[k]=0;
    }       
    
    do{
        item = cirQue[front];     // 队首元素出列
        front = (front+1) % (n+1);   // 移动队首指针（指向新的队首元素）
        
        if( front == prev ){
            groupNum++;
            result[item-1] = groupNum;
            for(k=0; k<n; k++)
                conf[k] = R[item-1][k];
            newRound = 0;
         }
         else if(conf[item-1] != 0){    //  若与当前组元素产生冲突，则再次入队
            cirQue[rear] = item;
            rear = (rear+1)%(n+1);            
        }
        else{
            result[item-1] = groupNum;
            for(k=0; k<n; k++)
                conf[k] = conf[k]+R[item-1][k];
        }
       
         prev = item;
     }while(rear != front);
}