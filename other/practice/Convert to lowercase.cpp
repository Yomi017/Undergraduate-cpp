#include <iostream>
using namespace std;

int main() {
    string a;
    cin >> a;

    // 处理输入字符串
    for (size_t i = 0; i < a.length(); ++i) {
        char c = a[i];
        if (c >= 'A' && c <= 'Z') { // 检查是否是大写字母
            c = c + 32; // 大写字母到小写字母的转换，ASCII 值相差 32
        }
        cout << c; // 输出转换后的字符
    }

    cout << endl; // 输出换行
    return 0;
}
