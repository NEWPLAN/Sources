#include "StdAfx.h"
#include<stdio.h>
#include<stdlib.h>
#include "string.h"

#if 0

// 顺序输出数组中的元素值
void output_array(int data[], int num)
{
	int i; 
	for ( i=1; i <= num; ++i){
		printf("%3d", data[i]);
	}
	printf("\n");
}

// 堆的筛选操作：令输入的序列保持堆的性质，结果为小顶堆
// 用途：删除堆顶元素，并将最后一个元素调换到堆顶后，恢复堆的性质
// 参数pbt为待筛选的序列（长度为num），是以pbt[i]为根的完全二叉树
// pbt中：以pbt[2i]和r[2i+1]为根的左、右子树是堆
// 注意：为了便于计算左右孩子的下标序号，数组编号从1开始（牺牲元素pbt[0]）

void sift_heap(int * pbt, int i, int num)
{
	int idx_child, tmp;

	tmp = pbt[i];  // 保存堆顶元素值

	idx_child = 2 * i;  // idx_child为节点i的左孩子序号

	while ( idx_child <= num ){
		if ( ( idx_child < num) && pbt[idx_child + 1] < pbt[idx_child]){
			// 如果当前节点的右孩子小于左孩子，则选择右孩子（作为交换对象）
			idx_child++;
		}

		if ( pbt[idx_child] >= tmp ){
			// 如果左右孩子的值均不小于当前节点值，则结束筛选
			break;
		}
		// 筛选操作
		// 交换当前节点与左右孩子中较小者的值
		pbt[i] = pbt[idx_child];
		// 以被交换的孩子节点为根，继续对其子树进行筛选
		i = idx_child;
		// 修改idx_child为当前节点i的左孩子序号
		idx_child = 2 * i;
	}
	
	pbt[i] = tmp;  // 将原来的堆顶元素“插入”到程序为其找出的“适当”位置
	return;
}

// 建堆：自底而上地调用sift_heap从一个完全二叉树（数组）pbt[1..size]构造一个小顶堆
void build_heap(int *pbt, int num)
{
	int i;
	for( i = (num /2); i >= 1; --i){
		sift_heap(pbt, i, num);
	}
}

// 堆排序
void heap_sort(int *pbt, int num)
{
	int i, tmp;
	//output_heap(pbt, size);
	
	for( i = num; i>=2; --i){
		tmp = pbt[1];      // 保存堆顶元素
		pbt[1] = pbt[i]; // 将队尾元素交换到堆顶
		pbt[i] = tmp;      // pbt[i]不再被访问，可用于保存排序结果（来自堆顶的当前最小值）

		// 队尾元素换到堆顶后，需执行筛选操作以保持堆的性质
		// 注意： 上述元素交换只是破坏了以pbt[1]为根的完全二叉树的小顶堆性质
		// 完成堆顶元素交换后，pbt[1]的左右子树仍然保持了小顶堆性质
		// 因此在创建堆时需要遍历size/2到1的结点逐一进行筛选，而这里只需要针对pbt[1]进行筛选即可
		sift_heap(pbt, 1, i-1);  // 执行筛选操作
	}

}

int main(void)
{
	int num;

	// 注意：为了便于计算左右孩子的下标序号，数组编号从1开始（牺牲元素pbt[0]）
	int pbt[] = {0, 49, 38, 65, 97, 76, 13, 27, 50};

	num = sizeof(pbt)/sizeof(pbt[0]) - 1; 

	// 从无序序列创建小顶堆
	build_heap(pbt, num);

	// 执行堆排序
	heap_sort(pbt, num);

	// 输出排序结果
	output_array(pbt, num);

	system("pause");
	return 0;
} 

#endif