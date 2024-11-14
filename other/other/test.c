#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

const int N = 3000005;
int primes[N];
bool not_prime[N];
int f[N];
int prime_count = 0;

void linear_sieve(int n) {
    f[0] = f[1] = 1;
    for (int i = 2; i <= n; ++i) {
        if (!not_prime[i]) {
            primes[prime_count++] = i;
            f[i] = 1;
        }
        for (int j = 0; j < prime_count; ++j) {
            int p = primes[j];
            if (i * p > n) break;
            not_prime[i * p] = true; 
            if (i % p) {
                f[i * p] = f[i] * f[p];
            } else {
                f[i * p] = 0;
                break;
            }
        }
    }
}

int main() {
    int n, N;
    scanf("%d %d", &n, &N);
    linear_sieve(N);
    int tot = 0;
    for (int i = n; i <= N; i++) {
        tot += 1 - f[i];
    }
    printf("%d\n", tot);
    return 0;
}