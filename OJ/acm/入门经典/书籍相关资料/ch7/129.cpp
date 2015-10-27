#include<stdio.h>
int n, L, cnt = 0;
int S[100];

int dfs(int cur) {                                       // ����0��ʾ�Ѿ��õ��⣬�����������
  if(cnt++ == n) {
    for(int i = 0; i < cur; i++) printf("%c", 'A'+S[i]); // �������
    printf("\n");
    return 0;
  }
  for(int i = 0; i < L; i++) {
    S[cur] = i;
    int ok = 1;
    for(int j = 1; j*2 <= cur+1; j++) {                  // ���Գ���Ϊj*2�ĺ�׺
      int equal = 1;
      for(int k = 0; k < j; k++)                         // ����һ���Ƿ����ǰһ��
        if(S[cur-k] != S[cur-k-j]) { equal = 0; break; }
      if(equal) { ok = 0; break; }                       // ��һ�����ǰһ�룬�������Ϸ�
    }
    if(ok) if(!dfs(cur+1)) return 0;                     // �ݹ�����������Ѿ��ҵ��⣬��ֱ���˳�
  }
  return 1;
}

int main() {
  scanf("%d%d", &n, &L);
  dfs(0);
  return 0;
}
