#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAP_SIZE (1000+3)
#define UNREACHABLE -1
#define INITSVAL  0x5FFFFFFF
int boundary=0;

int reachable(int imap[][MAP_SIZE] int px,int py, int boundary)
{
    return ((x<1)||(y<1)||(x>boundary)||(y>boundary)||(imap[x][y]!=UNREACHABLE));
}

int maps[MAP_SIZE][MAP_SIZE]={0};

void update(int x,int y)
{
    if(maps[x][y])
}
int main(int argc, char* argv[])
{
    int n,m,k,d;
    while(scanf("%d%d%d%d",&n,&m,&k,&d)!=EOF)
    {
        memset(maps,INITSVAL,sizeof(maps));
    }
    return 0;
}
