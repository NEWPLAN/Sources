#include<stdio.h>
int A[100];

// ���1~n��ȫ����
void print_permutation(int n, int* A, int cur) {
  int i, j;
  if(cur == n) { // �ݹ�߽�
    for(i = 0; i < n; i++) printf("%d ", A[i]);
    printf("\n");
  } else for(i = 1; i <= n; i++) { // ������A[cur]�����������i
    int ok = 1;
    for(j = 0; j < cur; j++)
      if(A[j] == i) ok = 0; // ���i�Ѿ���A[0]~A[cur-1]���ֹ���������ѡ
    if(ok) {
      A[cur] = i;
      print_permutation(n, A, cur+1); // �ݹ����
    }
  }
}

int main() {
  print_permutation(4, A, 0); 
  return 0;
}
