#include <bits/stdc++.h>
using namespace std;
int main(){
    int a;
    int b;
    cin >> a >> b;
    int c;
    c=a+b;
    cout << c << endl;
    c=a-b;
    cout << c << endl;
    c=a*b;
    cout << c << endl;
    if (b==0){
        cout << "undefined";
    }
    else{
        c=a/b;
        cout << c;
    }
    system("pause");
}