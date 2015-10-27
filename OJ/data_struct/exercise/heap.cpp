#include <iostream>
using namespace std;

#define max_inouts 100
int matrix[max_inouts]={0,1,3,5,8,6,2,9,5};

int head_size;
int N=8;
void bulid_heap(int*);
void heap_adjust(int*,int);
void head_sort(int*);
int main(void)
{
	/*cin>>N;
	for (int i=1;i<=N;i++)
	{
		cin>>matrix[i];
	}
	*/
	head_size=N;
	for(int i=1;i<=N;i++)
	{
		cout<<matrix[i]<<"   ";
	}
	cout<<"\ngenuis sort\n\n"<<endl;
	//bulid_heap(matrix);
	head_sort(matrix);
	for(int i=1;i<=N;i++)
	{
		cout<<matrix[i]<<"   ";
	}
	return 0;
}
void bulid_heap(int* matrix)
{
	for (int i=N/2;i>0;i--)
	{
		heap_adjust(matrix,i);
	}
}
void heap_adjust(int* matrix,int cur_pos)
{
	int lchild,rchild;
	lchild=2*cur_pos;
	rchild=2*cur_pos+1;
	int largest;
	if (lchild<=head_size && matrix[lchild]>matrix[cur_pos])
		largest=lchild;
	else
		largest=cur_pos;
	if(rchild<=head_size && matrix[rchild]>matrix[largest])
		largest=rchild;
	if (largest!=cur_pos)
	{
		swap(matrix[cur_pos],matrix[largest]);
		heap_adjust(matrix,largest);
	}
}

void head_sort(int* matrix)
{
	bulid_heap(matrix);
	for (int i=N;i>=2;i--)
	{
		swap(matrix[1],matrix[i]);
		head_size--;
		heap_adjust(matrix,1);
	}
}