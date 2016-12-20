#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include"windows.h"

#if 0

#define TMalloc(type,n)  (type *)malloc((n)*sizeof(type))
#define GPATH '#'            //�궨�嵱�Թ��ܹ���ͨʱͨ··�ߵ�ͼ��
#define GWALL '-'            //�궨�嵱�Թ��ܹ���ͨʱ��ͨ··�ߵ�ͼ��

/*
�ṹ��Tcell���ڱ�ʾ�Թ���һ�����Ϣ
node������Թ���Ѱ�ҳ�·ʱ��ǰ�ڵ��λ����Ϣ�� 
node_stack[]��Ŷ�ջ�еĸ����ڵ������ 
move[]������������ĸ���λ
*/
typedef struct cell
{
	int row;  //����к�
	int col;  //����к�
	int dir;  //���4����λ�� 0Ϊ�ң� 1Ϊ�£� 2Ϊ�� 3Ϊ��
}TCell, *TCellPtr;


int verify_mat(const char *, int*, int*);
int ** read_map(const char *, int, int); 
void show_map(int ** ,int, int);
void destroy_map(int ** ,int);
int ** setup_mat(int, int); 
TCellPtr setup_stack(int, int);
void search_maze(int **, int **, TCellPtr , int , int );
void show_route(int **, TCellPtr, int, int, int);


// Ѱ���Թ�·��������������ڳ�·����ѳ�·��ӡ����
void search_maze(int **maze, int **mark, TCellPtr pstack, int nrow, int ncol)
{
	int move[4][2] ={{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
	int top = 1;            // ·����ջ��ջ��ָ��
	int found = 0;          // ����Ƿ��ҵ��Թ���·��found=1��ʾ�ҵ���·
	int orientation = 0;    // orientation��ŵ�ǰԪ���������ķ�λ��0Ϊ�ң� 1Ϊ�£� 2Ϊ�� 3Ϊ��
	int next_row, next_col; // next_row, next_col�ֱ��ŵ�ǰԪ�ص���һ��Ԫ�ص�����ֵ
	TCell curcell;          // ��ǰ�Թ���Ԫ

	// ���Թ���ڵ�Ԫѹ��ջ����ע���Թ���Χ������һ�㡰��ǽ���������������Ϊ(1,1)��
	pstack[0].row = 1;      
	pstack[0].col = 1;
	pstack[0].dir = 0;

	mark[1][1] = 1;   // ���Թ����Ԫ�ؽ��б�ǣ������õ�Ԫ�Ѿ��߹�

	while(top >= 0 && !found){      // ���ջ�ǿգ�û���˻ص���ڵ�Ԫ��������û���ҵ��Թ����ڣ������Ѱ��·��
		curcell = pstack[top--];    // ��ջ��Ԫ�س�ջ
		orientation  = curcell.dir; // ȡջ��Ԫ�صķ���ֵΪ��ǰ����
		// ��curcell�ĵ�ǰ����Ϊ��㣬˳ʱ������ھӵ�Ԫ
		while(orientation < 4 && !found){ //��4������û�б����꣬����û���ҵ��Թ�����			
			next_row = curcell.row + move[orientation][0];  // ���㵱ǰ������ھӵ�Ԫ���к�
			next_col = curcell.col + move[orientation][1];  // ���㵱ǰ������ھӵ�Ԫ���к�

			if(next_row == nrow - 2 && next_col == ncol - 2){  
				found = 1;		//����ھӵ�Ԫǡ��Ϊ�Թ����ڣ���found��־��Ϊ1
			}
			else if(!maze[next_row][next_col] && !mark[next_row][next_col]) { 
				// ����ھӵ�Ԫ�ɴ����δ�����ʹ�
				mark[next_row][next_col] = 1;           // ���õ�Ԫ���Ϊ�ѷ���
				pstack[top].dir = ++orientation;        // ��ǰԪ�ص���һ������ֵ����ǰԪ��

				top++;	                        //����Ԫ�ط���ջ��	
				pstack[top].row = next_row;					
				pstack[top].col = next_col;
				pstack[top].dir = 0;

				curcell.row = next_row;
				curcell.col = next_col;
				orientation = 0;                        //�������dir����
				printf("%d  %d\n", next_row, next_col);
			}
			else
				orientation++;
		}// while(dir < 4 && !found)
	}//while(top > -1 && !found)

	//����ҵ��Թ���·������õ�ͼ������������������ʾ�����û����Թ�û�г�·
	if(found){	
		show_route(maze, pstack, nrow, ncol, top);
	}
	else{ 
		printf("\n ���Թ�û�г�·��\n");
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
		printf("�����Թ���ͼ�ļ����ļ���ȡʧ�ܣ����ͼÿ�е�Ԫ����Ŀ������\n");
		exit(0);
	}

	// �����ͼ�ļ�
	maze = read_map(filename, nrow_map, ncol_map);
	// �����Թ����ʱ�Ǿ���
	mark = setup_mat(nrow_map, ncol_map);
	// ��ʼ��·��̽���ջ
	pstack = setup_stack(nrow_map, ncol_map);

	search_maze( maze, mark, pstack, nrow_map, ncol_map );


	// ����Ϊ�洢�Թ���ͼ����̬������ڴ�
	destroy_map(maze, nrow_map);
	// ����Ϊ�洢�Թ����ʱ�Ƕ���̬������ڴ�
	destroy_map(mark, nrow_map);
	// ����·��̽���ջ
	free(pstack);

	system("pause");
	return 0;
}

// У���Թ���ͼ
// �������ܣ����ļ��ж��������Ϣ����֤ÿһ��Ԫ�ظ����Ƿ���ͬ��������򷵻�1��������򷵻�0
// ��������ֵ���ļ���ȡ�ɹ�����1��ͬʱ����ָ���޸Ĵ�����������Ͳ�����ֵΪ����������
int verify_mat(const char *filename, int* pnrow_map, int* pncol_map)
{
	errno_t err;
	int nrow_map = 0, ncol_map = 0, counter = 0;
	char temp;
	FILE *fp;

	err = fopen_s( &fp, filename, "r" );
	if( err == 0 ){  //������ܴ��ļ�����ʾ�û��Թ��ļ�δ׼���ã����˳�����
		printf( "The file 'map.txt' was opened successfully for read.\n" );		
	}
	else{
		printf( "The file 'map.txt' can not be opened!\n" );
		exit(0);
	}

	//���Թ�������кŴ���nrow_map�У��кŴ���ncol_map�У�������Թ����������Ԫ�����Ƿ����
	while(1) 
	{
		temp = fgetc(fp);
		if(temp == EOF) {
			// �ļ���ȡ�꣬����ѭ��
			if(counter != ncol_map){
				//����Թ�������кŲ���ȫ����ͬ����ʾ��ʾ����˳�����
				printf("\n �Թ���������һ�е�Ԫ�ظ���������ͬ�����޸� map.txt �е��Թ�ͼ�������г���");
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
		else{  // ���뻻�з�ʱ
			if( counter == ncol_map )
				counter = 0;
			else {
				if( ncol_map == 0 ){
					ncol_map = counter;
					counter = 0;
				}
				else{  
					//����Թ�������кŲ���ȫ����ͬ����ʾ��ʾ����˳�����
					printf("\n�Թ���������һ�е�Ԫ�ظ���������ͬ�����޸� map.txt �е��Թ�ͼ�������г���");
					return 0;
				}
			}
			nrow_map++;
		}
	}//while(1)

	*pnrow_map = nrow_map;
	*pncol_map = ncol_map;
	fclose(fp);	
	return 1; // ��ʾУ��ɹ�
}

// ���ļ�filename���������Թ���ͼ��ά����
int ** read_map(const char *filename, int row, int column)
{	
	int **maze, i;
	errno_t err;
	char temp;
	FILE *fp;

	err = fopen_s( &fp, filename, "r" );
	if( err == 0 ){  //������ܴ��ļ�����ʾ�û��Թ��ļ�δ׼���ã����˳�����
		printf( "The file 'map.txt' was opened successfully for read.\n" );		
	}
	else{
		printf( "The file 'map.txt' can not be opened!\n" );
		exit(0);
	}

	// Ϊ�Թ���ͼ����洢�ռ�
	maze = TMalloc(int *, row);
	if(!maze){
		printf( "Ϊ�����Թ���ͼ�����ڴ�ʧ��\n" );
		exit(0);
	}
	for(i=0;i<row;i++){
		maze[i] = TMalloc(int, column);
		if(!maze[i]){
			printf( "Ϊ�����Թ���ͼ�����ڴ�ʧ��\n" );
			exit(0);
		}
	}

	for(int i = 0; i < row ; i++){
		for(int j = 0; j < column ; j++){
			temp = fgetc(fp) - 48;  //���Թ��е�ASSIC��ת��������
			if(temp != 0 && temp != 1){
				printf("\n�ļ��е�����ֻ����0��1����鿴���޸ģ�");
				exit(0);
			}
			else{ //��ͼ�ļ��е���ֵû�д��󣬽�����뵽����maze[][]��
				maze[i][j] = temp; 
			}
			//mark[i][j] = 0;       //���ļ���ÿ��Ԫ�صı����������
			//node_stack[j + i * row].dir = 0; //��ջ�ķ���Ԫ������
		}
		fgetc(fp);
	}
	fclose(fp);	

	system("cls");  //����
	printf("\n����һ�� %d�� %d�� ���Թ�����\n", row, column);
	show_map(maze, row, column);
	return maze;
}

// ����Ϊ�洢�Թ���ͼ����̬������ڴ�
void destroy_map(int **my_array, int nrow)
{
	int i;
	for(i=0; i<nrow; i++)
		free(my_array[i]);
	free(my_array);
}

// ��ʾ������Թ�������Ϣ
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

// �����������������������̬���ɶ�ά���飬�����ȫ��
int ** setup_mat(int row, int column)
{
	int **matrix, i;
	// Ϊ��ά��������洢�ռ�
	matrix = TMalloc(int *, row);
	if(!matrix){
		printf( "Ϊ��ά���������ڴ�ʧ��\n" );
		exit(0);
	}
	for(i=0; i<row; i++){
		matrix[i] = TMalloc(int, column);
		if(!matrix[i]){
			printf( "Ϊ��ά���������ڴ�ʧ��\n" );
			exit(0);
		}
	}
	for(int i = 0; i < row ; i++){
		for(int j = 0; j < column ; j++){
			matrix[i][j] = 0;  //����ά������ÿ��Ԫ�صı����������
		}
	}
	return matrix;
}

TCellPtr setup_stack(int nrow, int ncol)
{
	TCellPtr ps;
	ps = TMalloc( TCell, nrow*ncol );
	if(!ps){
		printf( "Ϊ��ջ�����ڴ�ʧ��\n" );
		exit(0);
	}
	return ps;
}

void show_route(int **maze, TCellPtr pstack, int nrow, int ncol, int top)
{
	int i, j;
	int choice = 1; // ���ڱ������ѡ�� 
	int ** newmap = setup_mat( nrow, ncol); //newmap[][]����Թ��ĳ�··��ͼ�е��Թ���Ԫ 
	for(i = 0; i < nrow; i++)  
		for( j = 0; j < ncol; j++)
			newmap[i][j] = GWALL;  // �����ַ���#����������Թ�

	while(choice)
	{
		printf("\n��ѡ����ʾ��ʽ��\n  1.��ʾԭʼ�Թ�ͼ\n  2.��ʾ�Թ���··������\n  3.��ʾ�Թ���·ģ��ͼ\n  0.������Ϸ\n");
		printf("\n��ѡ����Ӧ������: ");
		scanf_s("%d", &choice);
		getchar();

		switch(choice){    //ѡ����ʾ��ʽ			
		case 0:
			break;
		case 1:
			show_map(maze, nrow, ncol);
			break;
		case 2:
			{
				printf("\n�Թ���··��Ϊ:");     //�����··��
				printf("\n��\t��");
				for( i = 0; i <= top; i++)
					printf("\n%d\t%d", pstack[i].row, pstack[i].col);
				printf("\n%d\t%d",nrow, ncol);
				//choice = 0;
				break;
			}
		case 3:
			{
				// ���÷��š�-�������·�����Ӧ���Թ���Ԫ
				for( i = 0; i <= top; i++){
					newmap[pstack[i].row][pstack[i].col] = GPATH;
				}
				newmap[nrow - 2][ncol - 2] = GPATH; // �����ڵ�Ԫ

				printf("\n�Թ��ĳ�·ģ����ʾͼ������\n");
				//����Թ���Խ·��ͼ
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
				printf("�������,���������룡\n");
				scanf_s("%d", &choice);
			}
		}//switch(choice)
	}//while(choice)
	destroy_map(newmap,  nrow); // ����Ϊ�Թ���ͼ������ڴ�ռ�
}

#endif

//// ������ýṹ���ʾ�Թ��Ļ�
//typedef struct
//{
//	int ** maze; // �Թ���ͼ����ά���飩
//	int nrow;    // �Թ�����
//	int ncol;    // �Թ�����
//}TMAP;