/*********************************************
*  P227 �㷨7.1  ֱ�Ӳ��������㷨
**********************************************/
typedef struct {  
    int key;
    float info;
}TNode;

// �Գ���Ϊnum�����а��ؼ�����������
// ���д�����R�±�Ϊ1��λ�ÿ�ʼ�洢�����鳤��Ϊnum+1
// R[0]������ʱ�洢�������Ԫ�أ��ӵڶ���λ�ô��ļ�¼��ʼ����
void sort_direct_insert(int R[], int num)  
{
    int i, idx;
    for( i=2; i<=num; ++i ){ 
        if (R[i] < R[i-1]){
            // ��R[i]�����������е����ֵ������鿴��һ��Ԫ��R[i+1]
            R[0] = R[i];  // R[0]��Ϊ������
            // �����򲿷����һ��λ�ÿ�ʼ��ǰ���ң�ͬʱ����Ԫ��
            // Ŀ����ΪR[i]�ҳ��ʵ��Ĳ���λ��
            for(idx = i-1; R[0] < R[idx]; --idx){
                R[idx+1] = R[idx];
            }
            R[idx+1] = R[0];
        }
    }
}

// �Խṹ������Ϊ������ֱ�Ӳ��������㷨
void sort_dinsert(TNode R[], int num)  
{
    int i, idx;
    for( i=2; i<=num; ++i ){ 
        if (R[i].key < R[i-1].key){
            // ��R[i]�����������е����ֵ������鿴��һ��Ԫ��R[i+1]
            R[0].key = R[i].key;
            R[0].info = R[i].info;  // R[0]��Ϊ������
            // �����򲿷����һ��λ�ÿ�ʼ��ǰ���ң�ͬʱ����Ԫ��
            // Ŀ����ΪR[i]�ҳ��ʵ��Ĳ���λ��
            for(idx = i-1; R[0].key < R[idx].key; --idx){
                R[idx+1].key = R[idx].key;
                R[idx+1].info = R[idx].info;
            }
            R[idx+1].key = R[0].key;
            R[idx+1].info = R[0].info;
        }        
    }
}

int main()
{
    int i, num;
    int keys[] = {0, 25, 3, 9, 36, 12, 45, 97, 7, 68, 32};
    float infos[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0 , 8.0, 9.0 , 10.0 };

    TNode mylist[11];    
    mylist[0].key = 0;
    mylist[0].info = 0.0;

    for(i = 0; i < 10; ++i){
        mylist[i+1].key = keys[i+1];
        mylist[i+1].info = infos[i];
        //printf("%3d, %3.2f\n", mylist[i+1].key, mylist[i+1].info);    
    }
    num = sizeof(infos)/sizeof(infos[0]);

    printf("����������key��ֵ�����������Ľ�����£�\n");
    sort_direct_insert(keys, num);
    for(i = 1; i < 11; ++i){
        printf("%2d\n", keys[i]);        
    }

    printf("�Խṹ�����鰴�ؼ��ֽ�������Ľ�����£�\n");
    sort_dinsert(mylist, num);
    for(i = 1; i < 11; ++i){
        printf("%2d, %5.2f\n", mylist[i].key, mylist[i].info);        
    }

    system("pause");
    return 0;
}