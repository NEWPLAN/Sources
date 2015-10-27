#include <stdio.h>
template <typename T>
class Heap
{
public:
	Heap():pt(NULL),counts(0){};
	~Heap(){}
	void Sort(T* arrs,int sizes=0);
	void Insert(T* arrs,T data);

private:
	void Adjust(T arrs);
	int counts;
	T *pt;
};

template<typename T>
void Heap<T>::Sort(T*arrs,int sizes=0)
{

}

template <typename T>
void Heap<T>::Insert(T* arrs,T data)
{

}
void Adjust(int *arrs,int begin,int ends);

void BuildHeap(int * arrs,int counts)
{
	int larges=counts/2;
	for (int i=counts/2;i>0;i--)
	{
		larges=2*i;
		if(larges<counts)
			if (arrs[larges]<arrs[larges+1])
				larges++;
		if (arrs[i]<arrs[larges])
		{
			arrs[i]+=arrs[larges];
			arrs[larges]=arrs[i]-arrs[larges];
			arrs[i]-=arrs[larges];
			Adjust(arrs,larges,counts);
		}
	}
}

void Adjust(int *arrs,int begin,int ends)
{
	int i,larges=begin,te=begin;
	for(i=begin;2*i<=ends;)
	{
		larges=2*i;
		if (larges<ends&&arrs[larges]<arrs[larges+1])
			larges++;
		if(arrs[i]<arrs[larges])
		{
			arrs[i]+=arrs[larges];
			arrs[larges]=arrs[i]-arrs[larges];
			arrs[i]-=arrs[larges];
			Adjust(arrs,larges,ends);
		}
		else
			return ;
		
	}
}

void HeapSort(int *arrs,int counts)
{
	BuildHeap(arrs,counts);
	int i=counts;
	while(i>1)
	{
		arrs[1]+=arrs[i];
		arrs[i]=arrs[1]-arrs[i];
		arrs[1]-=arrs[i];
		Adjust(arrs,1,--i);
	}	
}
int arrays[]={0,9,6,8,2,3,1,87,34,12,21,65};
int main(int argc,char** argv)
{
	
	HeapSort(arrays,sizeof(arrays)/sizeof(int)-1);
	for (int i=1;i<sizeof(arrays)/sizeof(int)-1;i++)
		printf("%d ",arrays[i]);
	return 0;
}