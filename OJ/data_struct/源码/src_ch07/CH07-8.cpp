/*********************************************
*  算法7.8  归并排序
**********************************************/
// 表R[s..t]的两部分R[s..m]和R[m+1..t]已按关键字有序
// 将R[s..m]和R[m+1..t]合并成有序表R1[s..t]
void merge(int Ra[], int Rb[], int start, int mid, int end)
{
	int i = start; 
	int j = mid+1; 
	int k = start;

	while( i <= mid && j <= end ){
		if(Ra[i] < Ra[j]){ 
			Rb[k++] = Ra[i++];
		}
		else {
			Rb[k++] = Ra[j++];
		}
	}
	while (i <= mid){
		Rb[k++] = Ra[i++];
	}
	while (j <= end){
		Rb[k++] = Ra[j++];
	}

	for (i = start; i <= end; i++){
		Ra[i] = Rb[i];  
	}
}


// 将序列Ra[start..end]归并排序为Rb[start..end]
void msort(int Ra[], int Rb[], int start, int end){
	int mid;
	if (start < end){		
		mid = (start + end) / 2;   // 根据mid将Ra等分成两部分
		msort(Ra, Rb, start, mid); // 对前半部分递归调用
		msort(Ra, Rb, mid+1, end); // 对后半部分递归调用
		merge(Ra, Rb, start, mid, end);  // 合并相邻的有序子序列
	}
}


// 二路归并排序算法：将序列R[1..n]归并排序
void sort_twoway_merge(int Ra[], int num){
	int i;
	int * Rb = (int *) malloc( sizeof(int) * num );
	if(!Rb){
		printf("为临时数组分配空间失败");
		exit(0);
	}
	
	msort(Ra, Rb, 0 , num-1);
	
	free(Rb);
}