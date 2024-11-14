#include <bits/stdc++.h>
using namespace std;

void slideLeft(vector<int>& r) {
    int n = r.size();
    vector<int> row(n, 0);
    int pos = 0;
    vector<bool> merged(n, false);

    for (int i = 0; i < n; ++i) {
        if (r[i] != 0) {
            if (pos > 0 && row[pos - 1] == r[i] && !merged[pos - 1]) {
                row[pos - 1] *= 2;
                merged[pos - 1] = true;
            } else {
                row[pos++] = r[i];
            }
        }
    }
    r = row;
}

void rotateClockwise(vector<vector<int>>& b) {
    int n = b.size();
    vector<vector<int>> board(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            board[j][n - 1 - i] = b[i][j];
        }
    }
    b = board;
}

void slideAndMerge(vector<vector<int>>& b, char direction) {
    int n = b.size();
    if (direction == 'L') {
        for (int i = 0; i < n; ++i) {
            slideLeft(b[i]);
        }
    } else if (direction == 'R') {
        for (int i = 0; i < n; ++i) {
            reverse(b[i].begin(), b[i].end());
            slideLeft(b[i]);
            reverse(b[i].begin(), b[i].end());
        }
    } else if (direction == 'U') {
        rotateClockwise(b);
        rotateClockwise(b);
        rotateClockwise(b);
        for (int i = 0; i < n; ++i) {
            slideLeft(b[i]);
        }
        rotateClockwise(b);
    } else if (direction == 'D') {
        rotateClockwise(b);
        for (int i = 0; i < n; ++i) {
            slideLeft(b[i]);
        }
        rotateClockwise(b);
        rotateClockwise(b);
        rotateClockwise(b);
    }
}

int main() {
    vector<vector<int>> b(4, vector<int>(4));
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            cin >> b[i][j];
        }
    }
    
    int N;
    cin >> N;
    
    vector<char> t(N);
    for (int i = 0; i < N; ++i) {
        cin >> t[i];
    }
    
    for (char op : t) {
        slideAndMerge(b, op);
    }
    
    for (const auto& r : b) {
        for (int c : r) {
            cout << c << " ";
        }
        cout << endl;
    }
    ;
}
