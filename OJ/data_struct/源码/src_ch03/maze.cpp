#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include"windows.h"

#if 0

#define TMalloc(type,n)  (type *)malloc((n)*sizeof(type))
#define GPATH '#'            //宏定义当迷宫能够走通时通路路线的图形
#define GWALL '-'            //宏定义当迷宫能够走通时非通路路线的图形

/*
结构体Tcell用于表示迷宫中一格的信息
node存放在迷宫中寻找出路时当前节点的位置信息， 
node_stack[]存放堆栈中的各个节点的数据 
move[]存放上下左右四个方位
*/
typedef struct cell
{
	int row;  //存放行号
	int col;  //存放列号
	int dir;  //存放4个方位， 0为右， 1为下， 2为左， 3为上
}TCell, *TCellPtr;


int verify_mat(const char *, int*, int*);
int ** read_map(const char *, int, int); 
void show_map(int ** ,int, int);
void destroy_map(int ** ,int);
int ** setup_mat(int, int); 
TCellPtr setup_stack(int, int);
void search_maze(int **, int **, TCellPtr , int , int );
void show_route(int **, TCellPtr, int, int, int);


// 寻找迷宫路径函数，如果存在出路，则把出路打印出来
void search_maze(int **maze, int **mark, TCellPtr pstack, int nrow, int ncol)
{
	int move[4][2] ={{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
	int top = 1;            // 路径堆栈的栈顶指针
	int found = 0;          // 标记是否找到迷宫出路，found=1表示找到出路
	int orientation = 0;    // orientation存放当前元素正遍历的方位，0为右， 1为下， 2为左， 3为上
	int next_row, next_col; // next_row, next_col分别存放当前元素的下一个元素的行列值
	TCell curcell;          // 当前迷宫单元

	// 将迷宫入口单元压入栈顶（注意迷宫外围增设了一层“外墙”，所以入口坐标为(1,1)）
	pstack[0].row = 1;      
	pstack[0].col = 1;
	pstack[0].dir = 0;

	mark[1][1] = 1;   // 对迷宫入口元素进行标记，表明该单元已经走过

	while(top >= 0 && !found){      // 如果栈非空（没有退回到入口单元），并且没有找到迷宫出口，则持续寻找路径
		curcell = pstack[top--];    // 将栈顶元素出栈
		orientation  = curcell.dir; // 取栈顶元素的方向值为当前方向
		// 以curcell的当前方向为起点，顺时针遍历邻居单元
		while(orientation < 4 && !found){ //当4个方向没有遍历完，并且没有找到迷宫出口			
			next_row = curcell.row + move[orientation][0];  // 计算当前方向的邻居单元的行号
			next_col = curcell.col + move[orientation][1];  // 计算当前方向的邻居单元的列号

			if(next_row == nrow - 2 && next_col == ncol - 2){  
				found = 1;		//如果邻居单元恰好为迷宫出口，将found标志置为1
			}
			else if(!maze[next_row][next_col] && !mark[next_row][next_col]) { 
				// 如果邻居单元可达，并且未被访问过
				mark[next_row][next_col] = 1;           // 将该单元标记为已访问
				pstack[top].dir = ++orientation;        // 当前元素的下一个方向赋值给当前元素

				top++;	                        //将此元素放入栈顶	
				pstack[top].row = next_row;					
				pstack[top].col = next_col;
				pstack[top].dir = 0;

				curcell.row = next_row;
				curcell.col = next_col;
				orientation = 0;                        //方向控制dir清零
				printf("%d  %d\n", next_row, next_col);
			}
			else
				orientation++;
		}// while(dir < 4 && !found)
	}//while(top > -1 && !found)

	//如果找到迷宫出路，则调用地图输出函数；否则给出提示告诉用户该迷宫没有出路
	if(found){	
		show_route(maze, pstack, nrow, ncol, top);
	}
	else{ 
		printf("\n 该迷宫没有出路！\n");
	}
}



int main()
{
	int nrow_map = 0, ncol_map = 0;
	const char *filename = "D:\\data\\map.txt";
	int **maze;
	int **mark;
	TCellPtr pstack;

	if(!verify_mat(filename, &nrow_map, &ncol_map)){
		printf("请检查迷宫地图文件：文件读取失败，或地图每行的元素数目不相容\n");
		exit(0);
	}

	// 读入地图文件
	maze = read_map(filename, nrow_map, ncol_map);
	// 生成迷宫访问标记矩阵
	mark = setup_mat(nrow_map, ncol_map);
	// 初始化路径探测堆栈
	pstack = setup_stack(nrow_map, ncol_map);

	search_maze( maze, mark, pstack, nrow_map, ncol_map );


	// 销毁为存储迷宫地图而动态分配的内存
	destroy_map(maze, nrow_map);
	// 销毁为存储迷宫访问标记而动态分配的内存
	destroy_map(mark, nrow_map);
	// 销毁路径探测堆栈
	free(pstack);

	system("pause");
	return 0;
}

// 校验迷宫地图
// 函数功能：从文件中读入矩阵信息，验证每一行元素个数是否相同，如果是则返回1，如果否则返回0
// 函数返回值：文件读取成功返回1，同时借助指针修改传入的两个整型参数的值为行数和列数
int verify_mat(const char *filename, int* pnrow_map, int* pncol_map)
{
	errno_t err;
	int nrow_map = 0, ncol_map = 0, counter = 0;
	char temp;
	FILE *fp;

	err = fopen_s( &fp, filename, "r" );
	if( err == 0 ){  //如果不能打开文件，提示用户迷宫文件未准备好，并退出程序
		printf( "The file 'map.txt' was opened successfully for read.\n" );		
	}
	else{
		printf( "The file 'map.txt' can not be opened!\n" );
		exit(0);
	}

	//将迷宫矩阵的行号存入nrow_map中，列号存入ncol_map中，并检查迷宫矩阵的行列元素数是否都相等
	while(1) 
	{
		temp = fgetc(fp);
		if(temp == EOF) {
			// 文件读取完，跳出循环
			if(counter != ncol_map){
				//如果迷宫矩阵的列号不是全都相同，显示提示语，并退出程序
				printf("\n 迷宫矩阵任意一列的元素个数必须相同，请修改 map.txt 中的迷宫图后再运行程序！");
				return 0;
			}
			else{
				nrow_map++;
				break;
			}
		}

		if(temp != '\n'){
			counter++;
		}
		else{  // 读入换行符时
			if( counter == ncol_map )
				counter = 0;
			else {
				if( ncol_map == 0 ){
					ncol_map = counter;
					counter = 0;
				}
				else{  
					//如果迷宫矩阵的行号不是全都相同，显示提示语，并退出程序
					printf("\n迷宫矩阵任意一行的元素个数必须相同，请修改 map.txt 中的迷宫图后再运行程序！");
					return 0;
				}
			}
			nrow_map++;
		}
	}//while(1)

	*pnrow_map = nrow_map;
	*pncol_map = ncol_map;
	fclose(fp);	
	return 1; // 表示校验成功
}

// 从文件filename出发建立迷宫地图二维数组
int ** read_map(const char *filename, int row, int column)
{	
	int **maze, i;
	errno_t err;
	char temp;
	FILE *fp;

	err = fopen_s( &fp, filename, "r" );
	if( err == 0 ){  //如果不能打开文件，提示用户迷宫文件未准备好，并退出程序
		printf( "The file 'map.txt' was opened successfully for read.\n" );		
	}
	else{
		printf( "The file 'map.txt' can not be opened!\n" );
		exit(0);
	}

	// 为迷宫地图申请存储空间
	maze = TMalloc(int *, row);
	if(!maze){
		printf( "为读入迷宫地图申请内存失败\n" );
		exit(0);
	}
	for(i=0;i<row;i++){
		maze[i] = TMalloc(int, column);
		if(!maze[i]){
			printf( "为读入迷宫地图申请内存失败\n" );
			exit(0);
		}
	}

	for(int i = 0; i < row ; i++){
		for(int j = 0; j < column ; j++){
			temp = fgetc(fp) - 48;  //将迷宫中的ASSIC码转换成数字
			if(temp != 0 && temp != 1){
				printf("\n文件中的内容只能是0或1，请查看并修改！");
				exit(0);
			}
			else{ //地图文件中的数值没有错误，将其存入到数组maze[][]中
				maze[i][j] = temp; 
			}
			//mark[i][j] = 0;       //将文件中每个元素的遍历标记清零
			//node_stack[j + i * row].dir = 0; //将栈的方向元素清零
		}
		fgetc(fp);
	}
	fclose(fp);	

	system("cls");  //清屏
	printf("\n这是一个 %d行 %d列 的迷宫矩阵！\n", row, column);
	show_map(maze, row, column);
	return maze;
}

// 销毁为存储迷宫地图而动态分配的内存
void destroy_map(int **my_array, int nrow)
{
	int i;
	for(i=0; i<nrow; i++)
		free(my_array[i]);
	free(my_array);
}

// 显示读入的迷宫数据信息
void show_map(int ** my_array, int row, int column)
{
	int i, j;
	for(i=0; i<row; i++)
	{
		for(j=0; j<column; j++)
		{
			//my_array[i][j]=i;
			printf("%2d", my_array[i][j]);
		}
		printf("\n");
	}
}

// 根据输入的行数和列数，动态生成二维数组，并填充全零
int ** setup_mat(int row, int column)
{
	int **matrix, i;
	// 为二维数组申请存储空间
	matrix = TMalloc(int *, row);
	if(!matrix){
		printf( "为二维数组申请内存失败\n" );
		exit(0);
	}
	for(i=0; i<row; i++){
		matrix[i] = TMalloc(int, column);
		if(!matrix[i]){
			printf( "为二维数组申请内存失败\n" );
			exit(0);
		}
	}
	for(int i = 0; i < row ; i++){
		for(int j = 0; j < column ; j++){
			matrix[i][j] = 0;  //将二维数组中每个元素的遍历标记清零
		}
	}
	return matrix;
}

TCellPtr setup_stack(int nrow, int ncol)
{
	TCellPtr ps;
	ps = TMalloc( TCell, nrow*ncol );
	if(!ps){
		printf( "为堆栈申请内存失败\n" );
		exit(0);
	}
	return ps;
}

void show_route(int **maze, TCellPtr pstack, int nrow, int ncol, int top)
{
	int i, j;
	int choice = 1; // 用于保存输出选项 
	int ** newmap = setup_mat( nrow, ncol); //newmap[][]存放迷宫的出路路径图中的迷宫单元 
	for(i = 0; i < nrow; i++)  
		for( j = 0; j < ncol; j++)
			newmap[i][j] = GWALL;  // 采用字符“#”填充整个迷宫

	while(choice)
	{
		printf("\n请选择显示方式：\n  1.显示原始迷宫图\n  2.显示迷宫出路路径坐标\n  3.显示迷宫出路模拟图\n  0.结束游戏\n");
		printf("\n请选择相应的数字: ");
		scanf_s("%d", &choice);
		getchar();

		switch(choice){    //选择显示方式			
		case 0:
			break;
		case 1:
			show_map(maze, nrow, ncol);
			break;
		case 2:
			{
				printf("\n迷宫出路路径为:");     //输出出路路径
				printf("\n行\t列");
				for( i = 0; i <= top; i++)
					printf("\n%d\t%d", pstack[i].row, pstack[i].col);
				printf("\n%d\t%d",nrow, ncol);
				//choice = 0;
				break;
			}
		case 3:
			{
				// 采用符号“-”填充与路径相对应的迷宫单元
				for( i = 0; i <= top; i++){
					newmap[pstack[i].row][pstack[i].col] = GPATH;
				}
				newmap[nrow - 2][ncol - 2] = GPATH; // 填充出口单元

				printf("\n迷宫的出路模拟显示图形如下\n");
				//输出迷宫穿越路线图
				for(i = 0; i < nrow; i++) {  
					for( j = 0; j < ncol; j++){
						printf("%c ", newmap[i][j]);
					}
					printf("\n");
				}
				break;
			}
		default:
			{
				printf("输入错误,请重新输入！\n");
				scanf_s("%d", &choice);
			}
		}//switch(choice)
	}//while(choice)
	destroy_map(newmap,  nrow); // 回收为迷宫地图分配的内存空间
}

#endif

//// 如果采用结构体表示迷宫的话
//typedef struct
//{
//	int ** maze; // 迷宫地图（二维数组）
//	int nrow;    // 迷宫行数
//	int ncol;    // 迷宫列数
//}TMAP;