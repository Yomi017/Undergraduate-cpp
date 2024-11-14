#include <iostream>
#include <vector>
#include <cmath>
#include <bitset>
using namespace std;
vector<int> primes;
bitset<3000000> is_prime;
void linear_sieve(int n) {
    is_prime.set();
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
void isHappy(int n,int N){
    int i,num,j,total = 0;
    for(int k=n;k<N+1;++k){
        for(int q = 0;q<primes.size();++q){
            i = 0;
            num = primes[q];
            if (num * num > k) break;
            j = k;
            while(true){
                if(j % num == 0){
                    j = j / num;
                    ++i;
                }
                else{
                    break;
                }
            }
            if(i>1){
                total = total + 1;
                break;
            }
        }
    }
    cout << total;
}
int main(){
    int n,N;
    cin >> n >> N;
    linear_sieve(sqrt(N));
    isHappy(n,N);
    cin>>N;
    return 0;
}
