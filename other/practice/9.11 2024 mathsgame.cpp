#include <bits/stdc++.h>
using namespace std;
int main(){
    string vect;
    cin >> vect;
    int b=0;
    string a;
    a="1";
    for(int i=0; i<8;++i){
        if(a[0]==vect[i]){
            b=b+1;
        }
    }
    cout << b;
}