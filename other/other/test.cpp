#include <iostream>
using namespace std;
int max(int x,int y);
int max(int x,int y,int z);
int main(){
    int a=5,b=10,c=15;
    cout << max(a,b) << endl;
    system("pause");
    return 0;
}
int max(int x, int y) {
    cout << "max(int x, int y)" << endl;
    return (x > y) ? x : y;  // 比较 x 和 y，返回较大值
}