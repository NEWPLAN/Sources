/***********************************************
 *  ũ��������⣨P64 ���е�Ӧ�ã� 
 *  Լ��������λ���������ֱ�˳���ʾũ���ǡ��ײ˺����״̬
 *     ����{dddd} <=> {Farmer, Wolf, Cabbage, Goat} ���У�d={0,1}
 *  ˵����0��ʾ�ڶ��� 1��ʾ����������ʼ״̬Ϊ0000����ֹ״̬Ϊ1111 
************************************************/
#include<stdio.h>
#include<stdlib.h>

#define MAXSIZE 16
typedef int EntryType;

typedef struct queue
{
    EntryType data[MAXSIZE];
    int front,rear;        //front��ͷ��rear��β
}SeqQueue, * SeqQueuePtr;

// �����ն���
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

// ���
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

// ����Ԫ�س��ӣ�����0��ʾ�����쳣�����Ӳ���ǰ����Ϊ�գ�
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

// ��״̬���ݣ���ȫ���򷵻�1�����򷵻�0
int is_safe(int state)
{
    if((is_goat_crossed(state) == is_cabbage_crossed(state)) && 
        (is_goat_crossed(state) != is_farmer_crossed(state)))  // ���ͬ����ũ���ڳ�
        return(0);
    if((is_goat_crossed(state) == is_wolf_crossed(state)) &&
        (is_goat_crossed(state) != is_farmer_crossed(state)))  // ����ͬ����ũ���ڳ�
        return(0);
    return(1);
}

void river_crossing_problem()
{
    int route[16];           // ��¼�Ѿ����ǹ���״̬    
    int state;               // ��¼��ǰʱ�̵�״̬��״̬��ŵĶ�������ʽ��״̬����
    int aftercross;          // ��¼���ǰ��ѡ�񣨶ɺӶ��󣩻ᵼ�µĽ��״̬
    int passenger;           // ��ʱ���������ڱ��ũ���ѡ�񣨶�Ӧ������λΪ1��ʾѡ�иó˿ͣ�

    int results[16]={0};     // ������
    int counter, i;    
    
    SeqQueuePtr states_que;  //

    states_que = create_sequeue(); // ������״̬������
    enqueue(states_que,0x00);      // ��ʼ״̬0000���

    for(int i = 0; i < 16; i++){
        route[i] = -1;
    }
    //route[0] = 0;

    while(!is_queEmpty(states_que) && (route[15] == -1))
    {
        if( !dequeue(states_que, &state) ){
            printf("Error: dequeue() - the queue is empty\n");
        }

        // ���ο���ũ����ܵ�ѡ��Я���򡢰ײ˺��ǣ��Լ�ũ��ֻ��ɺ�        
        for( passenger = 1; passenger<= 8; passenger <<= 1 )
        {        
            // ����ũ�������ڹ��ӣ���ũ����ӵ�Ҳֻ������ũ��ͬ��Ķ���
            if(((state & 0x08) != 0) == ((state & passenger) != 0)){
                // ���ũ���뵱ǰ�˿��ںӰ���ͬһ��
                aftercross = state^( 0x08|passenger ); // �ɺӺ�����
                if(is_safe(aftercross) && (route[aftercross] == -1)){
                    // ����ɺӺ�״̬��ȫ������״̬���
                    route[aftercross] = state;  // ����ǰ״̬��������¼��·�������У��±�����Ϊ����״ֵ̬��
                    enqueue(states_que, aftercross);
                }
            }
        }//end for()
    }//end while()

    // ������Ӳ��ԣ�0��ʾ�ڶ��� 1��ʾ����������ʼ״̬Ϊ0000����ֹ״̬Ϊ1111
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
                printf("ũ��Ӷ���������:");
            }
            else{
                printf("ũ�������������:");
            }
            switch(state^aftercross ){
            case 12:
                printf("���Ǵ�����\n");
                break;
            case 10:
                printf("�Ѳ˴�����\n");
                break;
            case 9:
                printf("���������\n");
                break;
            default:
                printf("ʲôҲ����\n");
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
