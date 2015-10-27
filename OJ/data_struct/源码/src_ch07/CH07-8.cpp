/*********************************************
*  �㷨7.8  �鲢����
**********************************************/
// ��R[s..t]��������R[s..m]��R[m+1..t]�Ѱ��ؼ�������
// ��R[s..m]��R[m+1..t]�ϲ��������R1[s..t]
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


// ������Ra[start..end]�鲢����ΪRb[start..end]
void msort(int Ra[], int Rb[], int start, int end){
	int mid;
	if (start < end){		
		mid = (start + end) / 2;   // ����mid��Ra�ȷֳ�������
		msort(Ra, Rb, start, mid); // ��ǰ�벿�ֵݹ����
		msort(Ra, Rb, mid+1, end); // �Ժ�벿�ֵݹ����
		merge(Ra, Rb, start, mid, end);  // �ϲ����ڵ�����������
	}
}


// ��·�鲢�����㷨��������R[1..n]�鲢����
void sort_twoway_merge(int Ra[], int num){
	int i;
	int * Rb = (int *) malloc( sizeof(int) * num );
	if(!Rb){
		printf("Ϊ��ʱ�������ռ�ʧ��");
		exit(0);
	}
	
	msort(Ra, Rb, 0 , num-1);
	
	free(Rb);
}