#include<stdio.h>

void print_subset(int n, int s) {  // ��ӡ{0, 1, 2, ..., n-1}���Ӽ�S
  for(int i = 0; i < n; i++)
    if(s&(1<<i)) printf("%d ", i); // ����������C���ԡ���0ֵ��Ϊ�桱�Ĺ涨
  printf("\n");
}

int main() {
  int n = 5;
  for(int i = 0; i < (1<<n); i++)  // ö�ٸ��Ӽ�����Ӧ�ı��� 0, 1, 2, ..., 2^n-1
    print_subset(n, i);
  return 0;
}
