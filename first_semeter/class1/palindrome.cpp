#include <iostream>
using namespace std;
bool isPalindrome(int x) {
    int yuanx = x;
    int huanx = 0;

    while (x > 0) {
        int digit = x % 10;
        huanx = huanx * 10 + digit;
        x /= 10;
    }

    return yuanx == huanx;
}

int main() {
    int x;
    cin >> x;
    if (isPalindrome(x)) {
        cout << "True";
    } else {
        cout << "False";
    }
    return 0;
}
