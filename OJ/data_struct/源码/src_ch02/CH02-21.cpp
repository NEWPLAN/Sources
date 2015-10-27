/*************************************
 *  ���ϻ����Ӽ��㷨
**************************************/
// ע��conf��resultά��Ϊn��cirQueά��Ϊn+1��������һ��Ԫ�أ�

void  division( int R[][N], int cirQue[],
                        int conf[], int result[],  int n)
{
    int k, item, prev, groupNum;
    
    // ѭ�����г�ʼ��
    for(k=0; k<n; k++) {
        cirQue[k] = k+1; 
    }
        
    front = 0;   // ����ָ��ָ��������ӵ�Ԫ��λ��
    rear = n;    // ��βָ��ָ������һ�����Դ洢���ݵ�λ��
    prev = rear;          // ��¼��ǰ�Ķ�βλ��
    groupNum = 0;     // �����ţ���1��ʼ��
    
    // ��ͻ�����ʼ��
    for(k=0; k<n; k++){
        conf[k]=0;
    }       
    
    do{
        item = cirQue[front];     // ����Ԫ�س���
        front = (front+1) % (n+1);   // �ƶ�����ָ�루ָ���µĶ���Ԫ�أ�
        
        if( front == prev ){
            groupNum++;
            result[item-1] = groupNum;
            for(k=0; k<n; k++)
                conf[k] = R[item-1][k];
            newRound = 0;
         }
         else if(conf[item-1] != 0){    //  ���뵱ǰ��Ԫ�ز�����ͻ�����ٴ����
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