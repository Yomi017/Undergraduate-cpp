#include <bits/stdc++.h>
using namespace std;
int main(){
    double x1,x2,y1,y2;
    cin>>x1>>y1>>x2>>y2;
    cout<<fixed<<setprecision(6)<<sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1))<<endl;
}