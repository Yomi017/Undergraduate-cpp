#include <iostream>
#include <string>
using namespace std;

int main() {
    string str = "123.45";
    double num = stod(str);  // 将字符串转换为 double 类型
    cout << num << endl;  // 输出: 123.45
    return 0;
}
