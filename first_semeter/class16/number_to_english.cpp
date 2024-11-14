#include <iostream>
#include <vector>
#include <string>

using namespace std;
const vector<string> below20 = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", 
                                "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", 
                                "seventeen", "eighteen", "nineteen"};
const vector<string> tens = { "", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"};
const vector<string> thousands = { "", "thousand", "million", "billion" };
string helper(int n) {
    if (n == 0) return "";
    
    string result = "";
    
    if (n >= 100) {
        result += below20[n / 100] + " hundred ";
        n %= 100;
    }
    
    if (n >= 20) {
        result += tens[n / 10];
        if (n % 10 != 0) {
            result += "-" + below20[n % 10];  // Add hyphen between tens and ones place
        }
        result += " ";
    } else if (n > 0) {
        result += below20[n] + " ";
    }
    
    return result;
}
string numberToWords(long long n) {
    if (n == 0) return "zero";
    
    if (n < 0) {
        return "negative " + numberToWords(-n);
    }
    
    string result = "";
    int thousandIndex = 0;
    while (n > 0) {
        if (n % 1000 != 0) {
            result = helper(n % 1000) + (thousands[thousandIndex] == "" ? "" : thousands[thousandIndex] + " ") + result;
        }
        n /= 1000;
        thousandIndex++;
    }
    while (result.back() == ' ') {
        result.pop_back();
    }
    
    return result;
}

int main() {
    long long n;
    cin >> n;
    
    cout << numberToWords(n) << endl;
    
    return 0;
}
