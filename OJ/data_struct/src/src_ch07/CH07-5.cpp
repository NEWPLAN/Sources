/*********************************************
*  P232 算法7.5  快速排序（交换排序算法）
**********************************************/
// 快速排序算法：对序列R[low ..high]进行快速排序
// 参数说明：待排序序列为R[1..num]，Dist[0..len-1]为增量序列
// 待排序列从数组R下标为1的位置开始存储，数组R长度为num+1
// R[0]用于临时存储待插入的元素，从第二个位置处的记录开始排序

void sort_quick( int R[], int low, int high)
{
    int idx;
    if(low < high){
        // 调用划分过程将R一分为二，以idx保存“划分元”的位置
        idx = partition(R, low, high);
        sort_quick(R, low, idx-1);    // 对低端序列执行递归调用
        sort_quick(R, idx+1, high);  // 对高端序列执行递归调用
    }
}

// 快速排序的一趟划分，低端为low，高端为high，划分元选为R[low]
int partition(int R[], int low, int high)
{
    R[0] = R[low]; // 暂存划分元，腾出R[low]的位置用于调整元素
    
    while(low < high){
        while( (low < high) && ( R[high] >= R[0] ) ){
            // 当R[high]不小于划分元时，保持元素R[high]的位置不变
            // 同时左移high“指针”（high所指的位置往低端移）
            high--;
        }
        
        // 当上述while循环结束时，意味着两种可能性：
        // 一种情况是low == high，说明找到了划分元的插入位置
        // 另一种情况是（low < high），但R[high] < R[0]
        // 这说明在划分元右侧找到了小于划分元的元素，应将其交换到low指示的位置上去
        if( low < high ){
            // 将找到的比划分元小的记录交换到低端，同时右移low“指针”
            R[low] = R[high];
            low++;
        }
        
        while( (low < high) && (R[low] <= R[0])){
            // 当R[low]不大于划分元时，保持元素R[low]的位置不变
            // 同时右移low“指针”（low所指的位置往高端移）
            low++;
        }
        
        if( low < high ) {
            // 较划分元大的记录交换到高端
            R[high] = R[low]; 
            high--;
        }  
        
    } // end while(low < high)
    
    R[low] = R[0];  // 将划分元记录插入到正确位置    
    
    return low;       // 返回划分元位置    
}