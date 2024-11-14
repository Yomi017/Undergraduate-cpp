#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m, k, id;
    cin >> n >> m >> k >> id;
    vector<vector<int>> n1(m, vector<int>(2));
    for (int i = 0; i < m; ++i) {
        cin >> n1[i][0] >> n1[i][1];
    }
    set<int> num;
    num.insert(id);
    for (int u = 0; u < k; ++u) {
        set<int> new_num = num; // 复制当前可达节点
        for (const auto& edge : n1) {
            if (num.count(edge[0])) {
                new_num.insert(edge[1]);
            }
            if (num.count(edge[1])) {
                new_num.insert(edge[0]);
            }
        }
        num = new_num; // 更新可达节点
    }
    cout << num.size();
}