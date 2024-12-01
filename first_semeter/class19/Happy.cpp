#include <bits/stdc++.h>
using namespace std;

bool happy(string n, unordered_set<string>& s) {
    int sum = 0;
    for (int i = 0; i < n.size(); i++) {
        sum += (n[i] - '0') * (n[i] - '0');
    }
    n = to_string(sum);
    
    if (s.find(n) != s.end()) {
        return false;
    }

    s.insert(n);

    if (n == "1") {
        return true;
    }

    return happy(n, s);
}

int main() {
    string n;
    cin >> n;
    unordered_set<string> s; 
    if (happy(n, s)) {
        cout << "True";
    } else {
        cout << "False";
    }
    return 0;
}
