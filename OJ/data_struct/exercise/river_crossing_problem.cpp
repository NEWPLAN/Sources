/***********************************************
 *  农夫过河问题（P64 队列的应用） 
 *  约定：用四位二进制数分别顺序表示农夫、狼、白菜和羊的状态
 *     即：{dddd} <=> {Farmer, Wolf, Cabbage, Goat} 其中：d={0,1}
 *  说明：0表示在东岸 1表示在西岸，初始状态为0000，终止状态为1111 
************************************************/
#include<stdio.h>
#include<stdlib.h>

#define MAXSIZE 16
typedef int EntryType;

typedef struct queue
{
    EntryType data[MAXSIZE];
    int front,rear;        //front队头，rear队尾
}SeqQueue, * SeqQueuePtr;

// 创建空队列
SeqQueuePtr create_sequeue(void)
{
    SeqQueuePtr pque;
    pque = (SeqQueuePtr)malloc(sizeof(SeqQueue));
    if(pque){    
        pque->front = 0;
        pque->rear = 0;
    }
    else{
        printf("Error: malloc() failed, out of memory!\n");
    }
    return(pque);
}

int is_queEmpty(SeqQueuePtr pque)
{
    return( pque->front == pque->rear );
}

int is_queFull(SeqQueuePtr pque)
{
    return( (pque->rear+1)%MAXSIZE  ==  pque->front);
}

// 入队
int enqueue(SeqQueuePtr pque, EntryType x)
{
    if(is_queFull(pque)){
        printf("Queue Overflow Error: trying to add an element onto a full queue\n");
        return 1;
    }
    else{
        pque->data[pque->rear] = x;
        pque->rear = (pque->rear + 1) % MAXSIZE;
        return 0;
    }    
}

// 队首元素出队（返回0表示出队异常，出队操作前队列为空）
int dequeue(SeqQueuePtr pque, EntryType * e)
{
    if(is_queEmpty(pque)){
        printf("Empty Queue.\n");
        return 0;
    }
    else{
        *e = pque->data[pque->front];
        pque->front = (pque->front + 1) % MAXSIZE;
        return 1;
    }
}

int is_farmer_crossed(int state)
{
    return ((state & 0x08) != 0);
}

int is_wolf_crossed(int state)
{
    return ((state & 0x04) != 0);
}

int is_cabbage_crossed(int state)
{
    return ((state & 0x02) != 0);
}

int is_goat_crossed(int state)
{
    return ((state & 0x01) != 0);
}

// 若状态相容（安全）则返回1，否则返回0
int is_safe(int state)
{
    if((is_goat_crossed(state) == is_cabbage_crossed(state)) && 
        (is_goat_crossed(state) != is_farmer_crossed(state)))  // 羊菜同岸且农夫不在场
        return(0);
    if((is_goat_crossed(state) == is_wolf_crossed(state)) &&
        (is_goat_crossed(state) != is_farmer_crossed(state)))  // 狼羊同岸且农夫不在场
        return(0);
    return(1);
}

void river_crossing_problem()
{
    int route[16];           // 记录已经考虑过的状态    
    int state;               // 记录当前时刻的状态（状态编号的二进制形式即状态本身）
    int aftercross;          // 记录渔夫当前的选择（渡河对象）会导致的结果状态
    int passenger;           // 临时变量，用于表达农夫的选择（对应二进制位为1表示选中该乘客）

    int results[16]={0};     // 输出结果
    int counter, i;    
    
    SeqQueuePtr states_que;  //

    states_que = create_sequeue(); // 创建“状态”队列
    enqueue(states_que,0x00);      // 初始状态0000入队

    for(int i = 0; i < 16; i++){
        route[i] = -1;
    }
    //route[0] = 0;

    while(!is_queEmpty(states_que) && (route[15] == -1))
    {
        if( !dequeue(states_que, &state) ){
            printf("Error: dequeue() - the queue is empty\n");
        }

        // 依次考虑农夫可能的选择：携带羊、白菜和狼，以及农夫只身渡河        
        for( passenger = 1; passenger<= 8; passenger <<= 1 )
        {        
            // 由于农夫总是在过河，随农夫过河的也只能是与农夫同侧的东西
            if(((state & 0x08) != 0) == ((state & passenger) != 0)){
                // 如果农夫与当前乘客在河岸的同一侧
                aftercross = state^( 0x08|passenger ); // 渡河后的情况
                if(is_safe(aftercross) && (route[aftercross] == -1)){
                    // 如果渡河后状态安全，则将其状态入队
                    route[aftercross] = state;  // 将当前状态的索引记录到路径数组中（下标索引为后续状态值）
                    enqueue(states_que, aftercross);
                }
            }
        }//end for()
    }//end while()

    // 输出过河策略：0表示在东岸 1表示在西岸，初始状态为0000，终止状态为1111
    if(route[15] != -1)
    {
        //printf("The reverse path is:\n");
        counter = 0;
        for(state = 15; state != 0; state = route[state]){            
            //printf("The state is: %d \n",state);
            results[counter] = state;
            counter++;            
            //if(state == 0) return;            
        }

        for(i = 0; i< counter; i++){
            state= results[i];
            aftercross = results[i+1];
            if(state & 0x08){
                printf("农夫从东岸到西岸:");
            }
            else{
                printf("农夫从西岸到东岸:");
            }
            switch(state^aftercross ){
            case 12:
                printf("把狼带过河\n");
                break;
            case 10:
                printf("把菜带过河\n");
                break;
            case 9:
                printf("把羊带过河\n");
                break;
            default:
                printf("什么也不带\n");
                break;
            }
        }
    }
    else{
        printf("No solution for this problem.\n");
    }
}

int main(void)
{
    river_crossing_problem();
    system("pause");
    return 0;
}
