#ifndef PROJECT_H
#define PROJECT_H
#include <bits/stdc++.h>
using namespace std;
enum class Token {
    EQUAL,          // =
    ASTERISK,       // *
    LPAREN,         // (
    RPAREN,         // )
    SEMICOLON,      // ;
    CREATE,
    USE,
    INSERT,
    DROP,
    SELECT,
    INNER_JOIN,
    UPDATE,
    SET,
    DELETE,
    DATABASE,
    TABLE,
    INTO,
    ON,
    FROM,
    WHERE,
    POINT,
    VALUES,         // VALUES 关键字
    IDENTIFIER,     // 标识符
    COMMA,          // ,
    NUMBER,         // 数字（整数或浮点数）
    STRING          // 字符串常量
};
unordered_map<string, Token> token_map = {
    {"=", Token::EQUAL},
    {"*", Token::ASTERISK},
    {"(", Token::LPAREN},
    {")", Token::RPAREN},
    {";", Token::SEMICOLON},
    {",", Token::COMMA},
    {".", Token::POINT},
    {"CREATE", Token::CREATE},
    {"USE", Token::USE},
    {"INSERT", Token::INSERT},
    {"DROP", Token::DROP},
    {"SELECT", Token::SELECT},
    {"INNER_JOIN", Token::INNER_JOIN},
    {"UPDATE", Token::UPDATE},
    {"SET", Token::SET},
    {"DELETE", Token::DELETE},
    {"DATABASE", Token::DATABASE},
    {"TABLE", Token::TABLE},
    {"INTO", Token::INTO},
    {"ON", Token::ON},
    {"FROM", Token::FROM},
    {"WHERE", Token::WHERE},
    {"VALUES", Token::VALUES}  // 新增 VALUES 关键字
};

string token_to_string(Token token) {
    switch (token) {
        case Token::EQUAL: return "Token::EQUAL";
        case Token::LPAREN: return "Token::LPAREN";
        case Token::RPAREN: return "Token::RPAREN";
        case Token::COMMA: return "Token::COMMA";
        case Token::POINT: return "Token::POINT";
        case Token::SEMICOLON: return "Token::SEMICOLON";
        case Token::ASTERISK: return "Token::ASTERISK";
        case Token::CREATE: return "Token::CREATE";
        case Token::USE: return "Token::USE";
        case Token::INSERT: return "Token::INSERT";
        case Token::DROP: return "Token::DROP";
        case Token::SELECT: return "Token::SELECT";
        case Token::INNER_JOIN: return "Token::INNER_JOIN";
        case Token::UPDATE: return "Token::UPDATE";
        case Token::SET: return "Token::SET";
        case Token::DELETE: return "Token::DELETE";
        case Token::DATABASE: return "Token::DATABASE";
        case Token::TABLE: return "Token::TABLE";
        case Token::INTO: return "Token::INTO";
        case Token::ON: return "Token::ON";
        case Token::FROM: return "Token::FROM";
        case Token::WHERE: return "Token::WHERE";
        case Token::VALUES: return "Token::VALUES";
        case Token::IDENTIFIER: return "Token::IDENTIFIER";
        case Token::NUMBER: return "Token::NUMBER";
        case Token::STRING: return "Token::STRING";
        default: return "Unknown Token";
    }
}

struct TokenWithValue {
    Token token;
    string value;
    
    TokenWithValue(Token t, const string& v) : token(t), value(v) {}
};

bool is_number(const string& s);
bool is_string_literal(const string& s);
vector<TokenWithValue> lex(const string& input,bool& inside_paren);
vector<vector<TokenWithValue>> lexfile(const string& filename);

#endif