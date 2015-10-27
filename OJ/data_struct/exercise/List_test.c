#include <stdio.h>

typedef enum
{
	FALSE , TRUE
}BOOL;

typedef struct da
{
	int series;
	char* datainfo;
}DataType;

typedef struct lis
{
	struct lis* next,*preious;
	DataType data;
}MyList,ListPtr;


ListPtr create_list(void);

void destroy_list(ListPtr);

ListPtr merge_list(void);

int list_insert(ListPtr,DataType);

ListPtr list_reject(DataType);



int main(void)
{
	return 0;
}

ListPtr create_list(void)
{
	return NULL;
}

void destroy_list(ListPtr)
{
	return ;
}

ListPtr merge_list(void)
{
	return NULL;
}

int list_insert(ListPtr,DataType)
{
	return 0;
}

ListPtr list_reject(DataType)
{
	return NULL;
}