#include <bits/stdc++.h>
using namespace std;

int cnt = 0;

void hanoi(int n, char a, char b, char c) {
    if (n == 1) {
        cnt++;
        cout << "move disk 1 from rod " << a << " to rod " << c << endl;
        return;
    }
    hanoi(n - 1, a, c, b);
    cnt++;
    cout << "move disk " << n << " from rod " << a << " to rod " << c << endl;
    hanoi(n - 1, b, a, c);
}

int main() {
    int n;  // a_n = 2a_{n-1} - 1
    cin >> n;
    int total_moves = pow(2, n) - 1;
    cout << total_moves << endl;
    hanoi(n, '1', '2', '3');
    return 0;
}