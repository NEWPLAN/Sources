#include<cstdio>
#include<algorithm>
using namespace std;
int main() {
  int n, p[10];
  scanf("%d", &n);
  for(int i = 0; i < n; i++) scanf("%d", &p[i]);
  sort(p, p+n); // ���򣬵õ�p����С����
  do {
    for(int i = 0; i < n; i++) printf("%d ", p[i]); // �������p
    printf("\n");
  } while(next_permutation(p, p+n)); // ����һ������
  return 0;
}
