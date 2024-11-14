#include <bits/stdc++.h>
using namespace std;

int main() {
    // 创建一个二维向量，用于存储乘法表
    vector<vector<int>> outer;
    
    // 生成乘法表
    for (int i = 1; i <= 10; i++) {
        // 创建一个一维向量，用于存储当前行的乘法结果
        vector<int> inner;
        for (int j = 1; j <= 10; j++)
            // 计算 i * j 的结果并添加到当前行向量中
            inner.push_back(i * j);
        
        // 将当前行向量添加到外层二维向量中
        outer.push_back(inner);
    }
    
    // 打印乘法表
    for (vector<vector<int>>::iterator iter = outer.begin(); iter != outer.end(); ++iter) {
        // 获取当前行的向量
        vector<int> inner = *iter;
        for (vector<int>::iterator it = inner.begin(); it != inner.end(); ++it)
            // 打印当前元素，设置宽度为 4 个字符，确保对齐
            cout << setw(4) << *it;
        // 打印换行符，开始新的一行
        cout << endl;
    }

    return 0;
}