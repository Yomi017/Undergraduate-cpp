#include <bits/stdc++.h>
using namespace std;

int main() {
    long double a, b, d;
    string c;
    cin >> a >> b >> c;

    // Set fixed precision for floating-point results
    cout << fixed << setprecision(3);

    if (c == "+") {
        cout << a + b;
    }
    else if (c == "-") {
        cout << a - b;
    }
    else if (c == "*") {
        cout << a * b;
    }
    else if (c == "/") {
        if (b == 0) {
            cout << "Error: Division by zero!";
        } else {
            cout << a / b;
        }
    }
    else if (c == "%") {
        if (b == 0) {
            cout << "Error: Division by zero!";
        } else {
            cout << static_cast<int>(a) % static_cast<int>(b);
        }
    }
    else if (c == "^") {
        d = pow(a, b);
        cout << d; 
    }
    else if (c == "r") {
        d = pow(a, 1 / b);
        cout << d;
    }
    else if (c == "min") {
        cout << min(a, b);
    }
    else if (c == "max") {
        cout << max(a, b);
    }
    else {
        cout << "Error: Invalid operator!";
    }

    return 0;
}
