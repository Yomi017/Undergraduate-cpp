#include<bits/stdc++.h>
using namespace std;
int main(){
    int x;
    cin >> x;
    int a[x];
    for(int i=0;i<x;i++){
        a[i]=i+1;
        }
    srand(time(NULL));
    random_shuffle(a,a+x);
    for(int i=0;i<x;i++){
        cout << a[i]<<" ";
        }
    return 0;
}