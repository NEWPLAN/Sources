#include <iostream>
#include <cstdio>
using namespace std;
const int maxn = 1001;
int mod = 1000000007;

int factorial[maxn], inverse[maxn], inversefactorial[maxn];

inline long long c(int m, int n) {
	return 1LL * factorial[m] * inversefactorial[n] % mod
		* inversefactorial[m - n] % mod;
}

int main() {
	factorial[0] = factorial[1] = 1;
	inverse[0] = inverse[1] = 1;
	inversefactorial[0] = inversefactorial[1] = 1;
	for (int i = 2; i < maxn; i++) {
		factorial[i] = 1LL * factorial[i - 1] * i % mod;
		inverse[i] = mod - 1LL * mod / i * inverse[mod % i] % mod;
		inversefactorial[i] = 1LL * inversefactorial[i - 1] * inverse[i] % mod;
	}
	int x;
	while (scanf("%d", &x) == 1) {
		int ans = 0;
		for (int i = 2; i <= x - 2; ++i)
			ans = (ans + c(x - 1, i) * (i - 1) * (x - i - 1)) % mod;
		cout << ans << endl;
	}
	return 0;
}
