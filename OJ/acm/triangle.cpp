#include <iostream>
#include <algorithm>

using namespace std;

#define MAX_LINE 100

int lines;
int board[MAX_LINE][MAX_LINE];
int **dataptr;

int max_array(int i,int j);
void test_recurise(void);
void read_file(void);
void dynamic_program(void);


int main(int argc, char** argv)
{
	
	read_file();
	dataptr=board;
	test_recurise();
	
	return 0;
}


void read_file(void)
{
	cin>>lines;
	for(int i=0;i<lines;i++)
		for(int j=0;j<=i;j++)
			cin>>board[i][j];
	return;
}


/*递归版本*/
int max_array(int i,int j)
{
	/*到达最底边，i从0开始，到达最后面时候，也就是到达叶节点*/
	if(i>=lines-1)
	{
		
		return board[i][j];
	}
	else
	{
		return max(max_array(i+1,j),max_array(i+1,j+1))+board[i][j];
	}
}

void dynamic_program(void)
{
	return ;
}


void test_recurise(void)
{
	cout<<max_array(0,0)<<endl;
}