#include <bits/stdc++.h>
#include "../project/project.h"
using namespace std;
string Getline(const string& filename){
    ifstream file(filename);
    if (!file.is_open()){
        cerr << "Error" << endl;
        return "";
    }
    string input, line;
    while(getline(file, line)){
        input += line + '\n';
    }
    return input;
}

Lexer<Token> keyword() {
    // 使用 pstring 解析器来识别不同的关键字，并将它们映射到 Token 类型
    Lexer<Token> INNER_JOINLexer = pstring<std::string>("INNER_JOIN").map([](const std::string&){ return Token::INNER_JOIN; });
    Lexer<Token> DATABASELexer = pstring<std::string>("DATABASE").map([](const std::string&){ return Token::DATABASE; });
    Lexer<Token> CREATELexer = pstring<std::string>("CREATE").map([](const std::string&){ return Token::CREATE; });
    Lexer<Token> INSERTLexer = pstring<std::string>("INSERT").map([](const std::string&){ return Token::INSERT; });
    Lexer<Token> SELECTLexer = pstring<std::string>("SELECT").map([](const std::string&){ return Token::SELECT; });
    Lexer<Token> UPDATELexer = pstring<std::string>("UPDATE").map([](const std::string&){ return Token::UPDATE; });
    Lexer<Token> DELETELexer = pstring<std::string>("DELETE").map([](const std::string&){ return Token::DELETE; });
    Lexer<Token> TABLELexer = pstring<std::string>("TABLE").map([](const std::string&){ return Token::TABLE; });
    Lexer<Token> WHERELexer = pstring<std::string>("WHERE").map([](const std::string&){ return Token::WHERE; });
    Lexer<Token> DROPLexer = pstring<std::string>("DROP").map([](const std::string&){ return Token::DROP; });
    Lexer<Token> INTOLexer = pstring<std::string>("INTO").map([](const std::string&){ return Token::INTO; });
    Lexer<Token> FROMLexer = pstring<std::string>("FROM").map([](const std::string&){ return Token::FROM; });
    Lexer<Token> USELexer = pstring<std::string>("USE").map([](const std::string&){ return Token::USE; });
    Lexer<Token> SETLexer = pstring<std::string>("SET").map([](const std::string&){ return Token::SET; });
    Lexer<Token> ONLexer = pstring<std::string>("ON").map([](const std::string&){ return Token::ON; });

    // 显式指定类型
    Lexer<Token> lexer1 = or_lexer(INNER_JOINLexer, DATABASELexer);
    Lexer<Token> lexer2 = or_lexer(lexer1, CREATELexer);
    Lexer<Token> lexer3 = or_lexer(lexer2, INSERTLexer);
    Lexer<Token> lexer4 = or_lexer(lexer3, SELECTLexer);
    Lexer<Token> lexer5 = or_lexer(lexer4, UPDATELexer);
    Lexer<Token> lexer6 = or_lexer(lexer5, DELETELexer);
    Lexer<Token> lexer7 = or_lexer(lexer6, TABLELexer);
    Lexer<Token> lexer8 = or_lexer(lexer7, WHERELexer);
    Lexer<Token> lexer9 = or_lexer(lexer8, DROPLexer);
    Lexer<Token> lexer10 = or_lexer(lexer9, INTOLexer);
    Lexer<Token> lexer11 = or_lexer(lexer10, FROMLexer);
    Lexer<Token> lexer12 = or_lexer(lexer11, USELexer);
    Lexer<Token> lexer13 = or_lexer(lexer12, SETLexer);
    Lexer<Token> lexer14 = or_lexer(lexer13, ONLexer);

    // 返回最终解析器
    return Lexer<Token>([=](const std::string& input) -> std::optional<std::pair<Token, std::string>> {
        return lexer14.parse(input);  // 调用最终组合的解析器
    });
}
