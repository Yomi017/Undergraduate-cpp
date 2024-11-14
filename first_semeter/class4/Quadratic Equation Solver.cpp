#include <bits/stdc++.h>
using namespace std;

void fountion(double a, double b, double c) {
    double d = (pow(b, 2) - 4 * a * c);
    double root1, root2;
    
    if (d > 0) {
        root1 = (-b + sqrt(d)) / (2 * a);
        root2 = (-b - sqrt(d)) / (2 * a);
        cout << "Distinct real roots" << endl;
        if (root1 > root2) {
            cout << root1 << endl;
            cout << root2;
        } else {
            cout << root2 << endl;
            cout << root1;
        }
    } else if (d == 0) {
        cout << "Identical real roots" << endl;
        cout << -b / (2 * a);
    } else {
        cout << "Complex roots" << endl;
        double r1 = -b / (2 * a);
        double r2 = sqrt(-d) / (2 * a);
        cout << fixed << setprecision(2) << r1 << " + " << setprecision(3) << r2 << "i" << endl;
        cout << fixed << setprecision(2) << r1 << " - " << setprecision(3) << r2 << "i";
    }
}

int main() {
    double a, b, c;
    cin >> a >> b >> c;
    fountion(a, b, c);
    return 0;
}