#include <iostream>
using namespace std;

int main() {
    int m, p, k;
    cin >> m >> p >> k;
    int chocolates = m / p;
    int wrappers = chocolates;
    while (wrappers >= k) {
        int new_chocolates = wrappers / k;
        chocolates += new_chocolates;
        wrappers = wrappers % k + new_chocolates;
    }
    
    cout << chocolates << endl;
    
    return 0;
}
