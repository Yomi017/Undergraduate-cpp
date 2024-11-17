#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const vector<string> magnitude ={"", "thousand", "million", "billion"};
const vector<string> OneToNine = {"", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
const vector<string> Tens = {"", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"};
const vector<string> Teens = {"ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};

ll helper(string word){
    for (int i = 0; i < 10; i++){
        if (OneToNine[i] == word) return i;
        if (Tens[i] == word) return 10*i;
        if (Teens[i] == word) return 10+i;
    }
    return 0;
}

ll EngToInt(string english){
    istringstream iss(english);
    string word;
    ll IntNum = 0;
    ll current = 0;
    bool ifNeg = false;
    while (iss >> word){
        if (word == "zero") return 0;
        if (word == "negative"){
            ifNeg = true;
            continue;
        }
        if (word.find("-") != string::npos){
            ll split = word.find("-");
            string splitted1 = word.substr(0, split);
            string splitted2 = word.substr(split+1);
            current += helper(splitted1) + helper(splitted2);
        }
        current += helper(word);
        if (word == "hundred") current *= 100;
        for (int i = 1; i < 4; i++){
            if (magnitude[i] == word) {
                current *= pow(10, 3*i);
                IntNum += current;
                current = 0;
            }
        }
        
    }
    IntNum += current;
    if (ifNeg){
        IntNum = -IntNum;
    }
    return IntNum;
}

int main(){
    string english;&
    getline(cin, english);
    ll IntNum = EngToInt(english);
    cout << IntNum;
    return 0;
}