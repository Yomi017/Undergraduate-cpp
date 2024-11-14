#include <iostream>
#include <vector>

class EightQueens {
public:
    void solve() {
        std::vector<int> queens(8, -1); // 用于存储每一行皇后的位置
        backtrack(queens, 0);
    }

private:
    void backtrack(std::vector<int>& queens, int row) {
        if (row == 8) {
            printBoard(queens);
            return;
        }

        for (int col = 0; col < 8; col++) {
            if (isSafe(queens, row, col)) {
                queens[row] = col; // 放置皇后
                backtrack(queens, row + 1); // 尝试下一行
                queens[row] = -1; // 回溯
            }
        }
    }

    bool isSafe(const std::vector<int>& queens, int row, int col) {
        for (int i = 0; i < row; i++) {
            // 检查同列和两条对角线
            if (queens[i] == col || 
                queens[i] - i == col - row || 
                queens[i] + i == col + row) {
                return false;
            }
        }
        return true;
    }

    void printBoard(const std::vector<int>& queens) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (queens[i] == j) {
                    std::cout << " Q ";
                } else {
                    std::cout << " . ";
                }
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};

int main() {
    EightQueens solver;
    solver.solve();
    system("pause");
    return 0;
}
