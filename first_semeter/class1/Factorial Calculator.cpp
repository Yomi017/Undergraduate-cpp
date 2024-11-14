#include <bits/stdc++.h>
using namespace std;
long long F(int n){
    if(n==0) return 1;
    return n*F(n-1);
}
int main(){
    int n;
    cin>>n;
    cout<<F(n);
    return 0;
}