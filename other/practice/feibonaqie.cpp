#include <bits/stdc++.h>
using namespace std;
int main(){
    int n;
    double a_n;
    int shuliang;
    cin >> shuliang;
    int b[shuliang];
    for(int i=0;i<shuliang;++i){
        cin >> n;
        a_n=(pow((1+sqrt(5)),n)-pow((1-sqrt(5)),n))/(sqrt(5))/(pow(2,n));
        static_cast<int>(a_n);
        b[i]=a_n;
    }
    for(int i=0;i<shuliang;++i){
        cout << b[i]<<endl;
    }
}