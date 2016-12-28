#include <stdio.h>
int Arrays[10]={2,4,5,1,0,-1,12,-5,11,44};
/***********************************************************/
void Bubble_Sort(int* Array,int sizes);/*Ã°ÅÝÅÅÐò*/
/***********************************************************/



/***********************************************************/
void chose_sort(int* Array, int sizes);/*Ñ¡ÔñÅÅÐò*/
/***********************************************************/



/***********************************************************/
void insert_sort(int* Array, int sizes);/*²åÈëÅÅÐò*/
void BInsertSort(int data[],int n);/*ÕÛ°ë²åÈëÅÅÐò*/
/***********************************************************/



/***********************************************************/
void quick_sort(int* Array, int low,int high);/*¿ìËÙÅÅÐò*/
int find_postion(int* Array, int low,int high);
/***********************************************************/




/***********************************************************/
void merge_sort(int* Array, int sizes);/*¹é²¢ÅÅÐò*/
void merge(int A[],int p,int q, int r);
void division(int A[],int l,int r);
/***********************************************************/


/***********************************************************/
void shellsort(int *data, int size);/*Ï£¶ûÅÅÐò*/
/***********************************************************/

void  print(int* Array, int sizes);

int main(int argc,char** argv)
{
	print(Arrays,10);
	//quick_sort(Arrays,0,9);
	//Bubble_Sort(Arrays,10);
	//merge_sort(Arrays,10);
	//insert_sort(Arrays,10);
	//BInsertSort(Arrays,10);
	shellsort(Arrays,10);
	print(Arrays,10);
	return 0;
}

void shellsort(int *data, int size)
{
	int gap,key,j,i;
	for (gap = size / 2; gap > 0; gap /= 2)
		for (i = gap; i < size; ++i)
		{

			key = data[i];
			for( j = i -gap; j >= 0 && data[j] > key; j -=gap)
			{
				data[j+gap] = data[j];
			}  
			data[j+gap] = key;
		}
}

void BInsertSort(int data[],int n)
{
	int low,high,mid;
	int temp,i,j;
	for(i=1;i<n;i++)
	{
		low=0;
		temp=data[i];// ±£´æÒª²åÈëµÄÔªËØ
		high=i-1;
		while(low<=high) //ÕÛ°ë²éÕÒµ½Òª²åÈëµÄÎ»ÖÃ
		{                       
			mid=(low+high)/2;
			if(data[mid]>temp)
				high=mid-1;
			else
				low=mid+1;
		}
		j = i;    
		while ((j > low) && (data[j - 1] > temp))    
		{    
			data[j] = data[j - 1];//½»»»Ë³Ðò    
			--j;    
		}    
		data[low] = temp;  

	} 

}


void insert_sort(int arr[], int n)
{
	int temp;
	int i,j;
	for (i = 1; i <n; i++)    
	{    
		temp = arr[i];
		j = i;    
		while ((j > 0) && (arr[j - 1] > temp))    
		{    
			arr[j] = arr[j - 1];//½»»»Ë³Ðò    
			--j;    
		}    
		arr[j] = temp;    
	}   

}


int find_postion(int* Array, int low,int high)
{
	int var=Array[low];
	while(low<high)
	{

		while(low<high && Array[high]>=var)
			high--;
		Array[low]=Array[high];
		while(low<high && Array[low]<=var)
			low++;
		Array[high]=Array[low];
	}
	Array[low]=var;
	return low;
}

void quick_sort(int* Array, int low,int high)/*¿ìËÙÅÅÐò*/
{
	int pos=0;
	if (low<high)
	{
		pos=find_postion(Array,low,high);
		quick_sort(Array,low,pos-1);
		quick_sort(Array,pos+1,high);
	}
}

void Bubble_Sort(int* Array,int sizes)
{
	int i=0,j;
	int temp_var;
	for (i=sizes-1;i>0;i--)
	{
		for (j=0;j<i;j++)
		{
			if (Array[j]>Array[j+1])/*Ã°ÅÝµ½´óµÄµØ·½È¥*/
			{
				temp_var=Array[j+1];
				Array[j+1]=Array[j];
				Array[j]=temp_var;
			}
		}
	}
}

void merge(int A[],int p,int q, int r)
{
	int A1[10];
	int A2[10];
	int i,j,k;
	for( i=0;i<q-p+1;i++)
		A1[i]=A[p+i];
	for( i=0;i<r-q;i++)
		A2[i]=A[q+1+i];
	for(i=0,j=0,k=p;i<(q-p+1)&&j<(r-q);k++)
	{
		if(A1[i]<=A2[j])
		{
			A[k]=A1[i];
			i++;
		}
		else
		{
			A[k]=A2[j];
			j++;
		}
	}
	for(i;i<q-p+1;i++,k++)
		A[k]=A1[i];

	for(j;j<r-q;j++,k++)
		A[k]=A2[j];

	return ;
}
void division(int A[],int l,int r)
{
	if(l==r)
		return ;
	division(A,l,(l+r)/2);
	division(A,(l+r)/2+1,r);
	merge(A,l,(l+r)/2,r);
	return ;
}

void merge_sort(int* Array, int sizes)
{
	division(Array,0,9);
}


void  print(int* Array, int sizes)
{
	int i=0;
	for(i=0;i<sizes;i++)
		printf("%d\t",Array[i]);
	printf("\n");
}