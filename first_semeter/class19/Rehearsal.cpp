// #include <bits/stdc++.h>
// using namespace std;
// vector<vector<int>> grid;
// vector<int> output;

// vector<vector<int>> create_grid(int n, int m) {
//     for (int i = 1; i <= n; i++) {
//         vector<int> row;
//         for (int j = 1; j <= m; j++) {
//             row.push_back((i - 1) * m + j);
//         }
//         grid.push_back(row);
//     }
//     return grid;
// }

// void grid_helper(string& command) {
//     istringstream iss(command);
//     int type, x, y;
//     iss >> type >> x >> y;
//     if (type == 1 && x <= grid.size() && y <= grid[x - 1].size()) {
//         int temp = grid[x - 1][0];
//         grid[x - 1].erase(grid[x - 1].begin());
//         grid[y - 1].push_back(temp);
//     } else if (type == 2 && x <= grid.size() && y <= grid[x - 1].size()) {
//         output.push_back(grid[x - 1][y - 1]);
//     } else if (type == 2 && (x > grid.size() || (x <= grid.size() && y > grid[x - 1].size()))){
//         output.push_back(-1);
//     } else {
//         cerr << "ERROR! Invalid command.\n";
//     }
// }

// int main() {
//     int n,m,q;
//     cin >> n >> m >> q;
//     cin.ignore();
//     string command;
//     create_grid(n, m);
//     for (int i = 0; i < q; ++i) {
//         getline(cin, command);
//         grid_helper(command);
//         command.clear();
//     }
//     for (const auto& value : output) {
//         cout << value << endl;
//     }
//     return 0;
// }
#include <bits/stdc++.h>
using namespace std;

vector<deque<int>> grid;
vector<int> output;
vector<int> offset;  // 偏移量

void create_grid(int n, int m) {
    grid.resize(n);
    offset.resize(n, 0); // 偏移量初始化为0
    for (int i = 0; i < n; ++i) {
        for (int j = 1; j <= m; ++j) {
            grid[i].push_back(i * m + j);
        }
    }
}

void grid_helper(string& command) {
    istringstream iss(command);
    int type, x, y;
    iss >> type >> x >> y;
    --x; --y;  // 转换为0-indexed

    if (type == 1) {  // 移动操作
        if (x < grid.size() && !grid[x].empty()) {
            int temp = grid[x].front();
            grid[x].pop_front();
            grid[y].push_back(temp);
            offset[x]++;  // 增加偏移量
        }
    } else if (type == 2) {  // 查询操作
        if (x < grid.size() && y < grid[x].size()) {
            output.push_back(grid[x][y]);
        } else {
            output.push_back(-1);
        }
    }
}

int main() {
    int n, m, q;
    cin >> n >> m >> q;
    cin.ignore();  // 清空输入缓冲区
    create_grid(n, m);

    string command;
    for (int i = 0; i < q; ++i) {
        getline(cin, command);
        grid_helper(command);
    }

    for (int val : output) {
        cout << val << endl;
    }

    return 0;
}
