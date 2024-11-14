#include <iostream>
#include <cmath>     
#include <iomanip>   
#include <algorithm> 
using namespace std;

int main()
{
    float a, b;
    string op1;
    cin >> a >> b >> op1;

    cout << fixed << setprecision(3);
    
    if (op1 == "min")
    {
        cout << min(a, b) << endl;
    }
    else if (op1 == "max")
    {
        cout << max(a, b) << endl;
    }
    else
    {
        char op = op1[0];
        
        if (op == '+')
        {
            cout << a + b << endl;
        }
        else if (op == '-')
        {
            cout << a - b << endl;
        }
        else if (op == '*')
        {
            cout << a * b << endl;
        }
        else if (op == '/')
        {
            if (b == 0)
            {
                cout << "Error: Division by zero!" << endl;
            }
            else
            {
                cout << a / b << endl;
            }
        }
        else if (op == '%')
        {

            cout << fmod(a, b) << endl;
        }
        else if (op == '^')
        {
            cout << pow(a, b) << endl;
        }
        else if (op == 'r')
        {
            if (b == 0)
            {
                cout << "Error: Cannot take the root with 0 as exponent!" << endl;
            }
            else
            {
                cout << pow(a, 1 / b) << endl;
            }
        }
        else
        {
            cout << "Error: Invalid operator!" << endl;
        }
    }

    return 0;
}
