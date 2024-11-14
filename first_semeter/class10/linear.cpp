#include <iostream>
#include <vector>
using namespace std;
// 线性筛函数，返回1到n之间的所有素数
vector<int> linearSieve(int n, vector<bool> &isPrime) { 
    vector<int> primes;
 // 用于存储找到的素数
// 标记数组，true 表示是素数
 isPrime[0] = isPrime[1] = false; // 0 和 1 不是素数 
// 从2开始遍历 
for (int i = 2; i <= 2*n; ++i) { 
    if (isPrime[i]) { 
        primes.push_back(i);
        }
 // 如果 i 是素数，加入素数列表 }
 // 用当前素数筛掉其倍数 
for (int p : primes) {
     if (i * p > 2*n) break; 
// 如果超出范围，停止筛选
 isPrime[i * p] = false; 
// 标记 i * p 为合数 
// 如果 p 是 i 的最小质因子，停止继续筛掉更大的倍数
 if (i % p == 0) break;
  } 
  } return primes; 
  }
 int main() { 
    int n;
    cin >> n;
    vector<bool> isPrime(2*n, true); 
    vector<int> primes = linearSieve(n, isPrime);
    for (int i=n+1;;i++){
        if(isPrime[i]){
            cout<<i<<endl;
            break;
        }
    }
 }
