#include <iostream>
#include <vector>
#include <windows.h>
using namespace std;
vector<int> primes;
vector<bool> is_prime;
void linear_sieve(int n) {
    is_prime.assign(n + 1, 1);
    is_prime[0] = is_prime[1] = 0;
    for (int i = 2; i <= n; ++i) {
        if (is_prime[i]) primes.push_back(i);
        for (int p : primes) {
            if (i * p > n) break;
            is_prime[i * p] = 0; 
            if (i % p == 0) break; 
        }
    }
}
int main() {
    int n;
    SetConsoleOutputCP(CP_UTF8);
    cout << "输入筛选范围上限: ";
    cin >> n;

    linear_sieve(n);

    cout << "范围内的素数有：\n";
    for (int prime : primes) {
        cout << prime << " ";
    }
    cout << endl;

    return 0;
}
