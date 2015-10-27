#include<stdio.h>
#include<string.h>
const int MAXN = 1000;
int n;

int mat[MAXN][MAXN], vis[MAXN][MAXN];
void dfs(int x, int y) {
  if(!mat[x][y] || vis[x][y]) return;     // �������ʹ�������ӣ����ߵ�ǰ�����ǰ�ɫ
  vis[x][y] = 1;                          // ���(x,y)�ѷ��ʹ�
  dfs(x-1,y-1); dfs(x-1,y); dfs(x-1,y+1);
  dfs(x-1,y);               dfs(x,y+1);
  dfs(x+1,y-1); dfs(x+1,y); dfs(x+1,y+1); // �ݹ������Χ�İ˸�����
}

int main() {
  char s[MAXN + 10];
  memset(mat, 0, sizeof(mat));  // ���и��Ӷ���ʼ��Ϊ��ɫ��������ΧһȦ���������
  memset(vis, 0, sizeof(vis));  // ���и��Ӷ�û�з��ʹ�
  scanf("%d", &n);
  for(int i = 0; i < n; i++) {
    scanf("%s", s);
    for(int j = 0; j < n; j++)
      mat[i+1][j+1] = s[j]-'0'; // ��ͼ�����м��ƶ�һ�㣬�ճ�һȦ�׸���
  }
  int count = 0;
  for(int i = 1; i <= n; i++)
    for(int j = 1; j <= n; j++)
      if(!vis[i][j] && mat[i][j]) { count++; dfs(i,j); } // �ҵ�û�з��ʹ��ĺڸ�
  printf("%d\n", count);
  return 0;
}
