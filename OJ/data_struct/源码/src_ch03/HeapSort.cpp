#include "StdAfx.h"
#include<stdio.h>
#include<stdlib.h>
#include "string.h"

#if 0

// ˳����������е�Ԫ��ֵ
void output_array(int data[], int num)
{
	int i; 
	for ( i=1; i <= num; ++i){
		printf("%3d", data[i]);
	}
	printf("\n");
}

// �ѵ�ɸѡ����������������б��ֶѵ����ʣ����ΪС����
// ��;��ɾ���Ѷ�Ԫ�أ��������һ��Ԫ�ص������Ѷ��󣬻ָ��ѵ�����
// ����pbtΪ��ɸѡ�����У�����Ϊnum��������pbt[i]Ϊ������ȫ������
// pbt�У���pbt[2i]��r[2i+1]Ϊ�������������Ƕ�
// ע�⣺Ϊ�˱��ڼ������Һ��ӵ��±���ţ������Ŵ�1��ʼ������Ԫ��pbt[0]��

void sift_heap(int * pbt, int i, int num)
{
	int idx_child, tmp;

	tmp = pbt[i];  // ����Ѷ�Ԫ��ֵ

	idx_child = 2 * i;  // idx_childΪ�ڵ�i���������

	while ( idx_child <= num ){
		if ( ( idx_child < num) && pbt[idx_child + 1] < pbt[idx_child]){
			// �����ǰ�ڵ���Һ���С�����ӣ���ѡ���Һ��ӣ���Ϊ��������
			idx_child++;
		}

		if ( pbt[idx_child] >= tmp ){
			// ������Һ��ӵ�ֵ����С�ڵ�ǰ�ڵ�ֵ�������ɸѡ
			break;
		}
		// ɸѡ����
		// ������ǰ�ڵ������Һ����н�С�ߵ�ֵ
		pbt[i] = pbt[idx_child];
		// �Ա������ĺ��ӽڵ�Ϊ��������������������ɸѡ
		i = idx_child;
		// �޸�idx_childΪ��ǰ�ڵ�i���������
		idx_child = 2 * i;
	}
	
	pbt[i] = tmp;  // ��ԭ���ĶѶ�Ԫ�ء����롱������Ϊ���ҳ��ġ��ʵ���λ��
	return;
}

// ���ѣ��Ե׶��ϵص���sift_heap��һ����ȫ�����������飩pbt[1..size]����һ��С����
void build_heap(int *pbt, int num)
{
	int i;
	for( i = (num /2); i >= 1; --i){
		sift_heap(pbt, i, num);
	}
}

// ������
void heap_sort(int *pbt, int num)
{
	int i, tmp;
	//output_heap(pbt, size);
	
	for( i = num; i>=2; --i){
		tmp = pbt[1];      // ����Ѷ�Ԫ��
		pbt[1] = pbt[i]; // ����βԪ�ؽ������Ѷ�
		pbt[i] = tmp;      // pbt[i]���ٱ����ʣ������ڱ��������������ԶѶ��ĵ�ǰ��Сֵ��

		// ��βԪ�ػ����Ѷ�����ִ��ɸѡ�����Ա��ֶѵ�����
		// ע�⣺ ����Ԫ�ؽ���ֻ���ƻ�����pbt[1]Ϊ������ȫ��������С��������
		// ��ɶѶ�Ԫ�ؽ�����pbt[1]������������Ȼ������С��������
		// ����ڴ�����ʱ��Ҫ����size/2��1�Ľ����һ����ɸѡ��������ֻ��Ҫ���pbt[1]����ɸѡ����
		sift_heap(pbt, 1, i-1);  // ִ��ɸѡ����
	}

}

int main(void)
{
	int num;

	// ע�⣺Ϊ�˱��ڼ������Һ��ӵ��±���ţ������Ŵ�1��ʼ������Ԫ��pbt[0]��
	int pbt[] = {0, 49, 38, 65, 97, 76, 13, 27, 50};

	num = sizeof(pbt)/sizeof(pbt[0]) - 1; 

	// ���������д���С����
	build_heap(pbt, num);

	// ִ�ж�����
	heap_sort(pbt, num);

	// ���������
	output_array(pbt, num);

	system("pause");
	return 0;
} 

#endif