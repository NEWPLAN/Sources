/*
 *邻接矩阵广度遍历算法
 */
 
 void BFSTraverse(HGraph G)
 {
	int i,j;
	Queue Q;
	
	for(i=0;i<G.numVertexse;i++)
	{
		visited[i]=FALSE;
	}
	
	initQueue(&Q);
	
	for(i=0;i<G.numVertexse;i++)
	{
		if(!visited[i])
		{
			printf("%c ",G.vex[i]);
			visited[i]=TRUE;
			EnQueue(&Q,i);
			
			while(!QueueEmpty(Q))
			{
				DeQueue(&Q,i);
				for(j=0;i<G.numVertexse;j++)
				{
					if(G.art[i][j]==1&&!visited[j])
					{
						printf("%c ",G.vex[j]);
						visited[j]=TRUE;
						EnQueue(&Q,j);
					}
						
				}
			}
		}
	}
 }