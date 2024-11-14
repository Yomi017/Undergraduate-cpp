#include <iostream>
using namespace std;

class Time {
    public:
        int hh, mm, ss;
        Time() : hh(23), mm(59), ss(59) {
            cout << "Time object created!" << endl;
        }
};