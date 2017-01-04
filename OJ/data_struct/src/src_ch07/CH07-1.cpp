/*********************************************
*  P227 算法7.1  直接插入排序算法
**********************************************/
typedef struct {  
    int key;
    float info;
}TNode;

// 对长度为num的序列按关键字升序排序
// 序列从数组R下标为1的位置开始存储，数组长度为num+1
// R[0]用于临时存储待插入的元素，从第二个位置处的记录开始排序
void sort_direct_insert(int R[], int num)  
{
    int i, idx;
    for( i=2; i<=num; ++i ){ 
        if (R[i] < R[i-1]){
            // 若R[i]大于有序序列的最大值则继续查看下一个元素R[i+1]
            R[0] = R[i];  // R[0]作为监视哨
            // 从有序部分最后一个位置开始向前查找，同时右移元素
            // 目的是为R[i]找出适当的插入位置
            for(idx = i-1; R[0] < R[idx]; --idx){
                R[idx+1] = R[idx];
            }
            R[idx+1] = R[0];
        }
    }
}

// 以结构体数组为参数的直接插入排序算法
void sort_dinsert(TNode R[], int num)  
{
    int i, idx;
    for( i=2; i<=num; ++i ){ 
        if (R[i].key < R[i-1].key){
            // 若R[i]大于有序序列的最大值则继续查看下一个元素R[i+1]
            R[0].key = R[i].key;
            R[0].info = R[i].info;  // R[0]作为监视哨
            // 从有序部分最后一个位置开始向前查找，同时右移元素
            // 目的是为R[i]找出适当的插入位置
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

    printf("对整型数组key按值升序进行排序的结果如下：\n");
    sort_direct_insert(keys, num);
    for(i = 1; i < 11; ++i){
        printf("%2d\n", keys[i]);        
    }

    printf("对结构体数组按关键字进行排序的结果如下：\n");
    sort_dinsert(mylist, num);
    for(i = 1; i < 11; ++i){
        printf("%2d, %5.2f\n", mylist[i].key, mylist[i].info);        
    }

    system("pause");
    return 0;
}