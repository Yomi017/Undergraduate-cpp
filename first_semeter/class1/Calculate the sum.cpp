#include <bits/stdc++.h>
using namespace std;
int main(){
    long long a,b;
    cin >> a >> b;
    long long sum=a;
    for(int i=a+1;i<=b;++i){
        sum=sum+i;
    }
    cout << sum;
}