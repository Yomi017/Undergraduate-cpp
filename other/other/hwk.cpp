#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
const int N=3e6+5;
vector<int> primes;
bool not_prime[N];
int f[N];
void linear_sieve(int n) {
    f[0]=f[1]=1;
    for (int i = 2; i <= n; ++i) {
        if (!not_prime[i]) primes.push_back(i),f[i]=1;
        for (int p : primes) {
            if (i * p > n) break;
            not_prime[i * p] = 1; 
            if (i % p){  
                f[i*p]=f[i]*f[p];
            }
            else{
                f[i*p]=0;
                break;
            }
        }
    }
}
int main(){
    int n,N;
    cin >> n >> N;
    linear_sieve(N);
    int tot=0;
    for(int i=n;i<=N;i++) tot+=1-f[i];
    cout<<tot<<endl;
    system("pause");
    return 0;
}