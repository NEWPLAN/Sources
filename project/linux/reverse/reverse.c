#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INOUT_LEN 128

typedef struct inputList
{
    int inputLen;
    struct inputList* next;
    char* ptr;
}InputList,*InputPtr;

static int nu=0;
InputPtr currentPtr;

InputPtr CreateNode(void)
{
    InputPtr ps=(InputPtr)malloc(sizeof(InputList));
    if(ps)
    {
        ps->inputLen=0;
        ps->next=NULL;
        ps->ptr=NULL;
        return ps;
    }
    printf("fatal error in create node exit\n");
    exit(0);
    return 0;
}

void ReadFromStream(void)
{
	FILE * ps=fopen("Makefile","rb");
    InputPtr node=NULL;
read:
    node=CreateNode();
    if(!node->ptr)
    	node->ptr=realloc(node->ptr,INOUT_LEN);
again:
    if(!fgets(node->ptr,strlen(node->ptr)-1,ps))
    {
        if(feof(ps))
            return ;
        node->ptr=realloc(node->ptr,strlen(node->ptr)+INOUT_LEN);
        goto again;
    }
    nu++;
    node->inputLen=strlen(node->ptr);
    node->ptr[node->inputLen]=0;
    node->next=currentPtr;
    currentPtr=node;
    goto read;

}

void DisPlay(void)
{
    if(currentPtr)
    {
        printf("%d \t",nu--);
        printf("%s\n",currentPtr->ptr);
        currentPtr=currentPtr->next;
    }
}

int main(int argc,char *argv[])
{
    ReadFromStream();
    DisPlay();
    return 0;
}
