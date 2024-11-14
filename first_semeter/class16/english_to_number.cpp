#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>

using namespace std;

// Define Token enumeration
enum class Token {
    IDENTIFIER,     // 标识符
    NUMBER,         // 数字（例如 one, two, three 等）
    UNIT,           // 单位（例如 thousand, million 等）
    NEGATIVE        // 负号
};

// Function to convert Token to string for debugging purposes
string token_to_string(Token token) {
    switch (token) {
        case Token::IDENTIFIER: return "Token::IDENTIFIER";
        case Token::NUMBER: return "Token::NUMBER";
        case Token::UNIT: return "Token::UNIT";
        case Token::NEGATIVE: return "Token::NEGATIVE";
        default: return "Unknown Token";
    }
}

// Structure to hold the token and its corresponding value
struct TokenWithValue {
    Token token;
    string value;
    
    TokenWithValue(Token t, const string& v) : token(t), value(v) {}
};

// Define mappings for words to their respective numeric values
unordered_map<string, int> wordToNum = {
    {"zero", 0}, {"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5},
    {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}, {"ten", 10}, 
    {"eleven", 11}, {"twelve", 12}, {"thirteen", 13}, {"fourteen", 14}, {"fifteen", 15},
    {"sixteen", 16}, {"seventeen", 17}, {"eighteen", 18}, {"nineteen", 19},
    {"twenty", 20}, {"thirty", 30}, {"forty", 40}, {"fifty", 50}, {"sixty", 60},
    {"seventy", 70}, {"eighty", 80}, {"ninety", 90}, {"hundred", 100},
    {"thousand", 1000}, {"million", 1000000}, {"billion", 1000000000}
};

// Function to convert a number part (less than 1000) into its integer value
int parsePart(const vector<TokenWithValue>& tokens) {
    int result = 0;
    int current = 0;
    
    for (const auto& token : tokens) {
        if (token.token == Token::NUMBER) {
            current += wordToNum[token.value];
        } else if (token.token == Token::UNIT) {
            if (token.value == "hundred") {
                current *= 100;
            } else {
                result += current * wordToNum[token.value];
                current = 0;
            }
        }
    }
    
    result += current;
    return result;
}

// Function to process the tokens and convert the English words to a number
long long wordsToNumber(const string& s) {
    stringstream ss(s);
    string word;
    vector<TokenWithValue> tokens;
    
    // Tokenize the input string
    while (ss >> word) {
        if (word == "negative") {
            tokens.push_back(TokenWithValue(Token::NEGATIVE, word));
        } else if (wordToNum.find(word) != wordToNum.end()) {
            if (word == "hundred" || word == "thousand" || word == "million" || word == "billion") {
                tokens.push_back(TokenWithValue(Token::UNIT, word));
            } else {
                tokens.push_back(TokenWithValue(Token::NUMBER, word));
            }
        }
    }
    
    // Check if the number is negative
    bool isNegative = false;
    if (!tokens.empty() && tokens[0].token == Token::NEGATIVE) {
        isNegative = true;
        tokens.erase(tokens.begin());  // Remove "negative" token
    }
    
    long long result = 0;
    int current = 0;
    
    // Process the tokens
    vector<TokenWithValue> currentTokens;
    for (const auto& token : tokens) {
        if (token.token == Token::UNIT) {
            currentTokens.push_back(token);
            result += parsePart(currentTokens);
            currentTokens.clear();
        } else {
            currentTokens.push_back(token);
        }
    }
    
    if (!currentTokens.empty()) {
        result += parsePart(currentTokens);
    }
    
    return isNegative ? -result : result;
}

int main() {
    string input;
    cout << "Enter number in words: ";
    getline(cin, input);  // Read full line of input
    
    cout << "Number is: " << wordsToNumber(input) << endl;
    
    return 0;
}
