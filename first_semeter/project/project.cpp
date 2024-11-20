#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cctype>
#include <algorithm>
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
    STRING,          // 字符串常量
    GT,
    LT,
    AND,
    OR
};

// 列的定义
class Column {
public:
    string name;        // 列名
    string type;        // 列的数据类型
    bool is_primary_key = false;  // 是否是主键
    bool is_not_null = false;     // 是否为 NOT NULL

    Column(string name, string type)
        : name(name), type(type) {}

    // 设置主键约束
    void set_primary_key() {
        is_primary_key = true;
    }

    // 设置 NOT NULL 约束
    void set_not_null() {
        is_not_null = true;
    }

    // 输出列信息
    // void print() const {
    //     cout << name << " " << type;
    //     if (is_not_null) cout << " NOT NULL";
    //     if (is_primary_key) cout << " PRIMARY KEY";
    //     cout << endl;
    // }
};

// 表的定义
class Table {
public:
    string name;               // 表名
    vector<Column> columns;    // 列
    vector<vector<string>> data; // 数据
    Table() : name("") {}
    Table(string name) : name(name) {}

    // 添加列
    // void add_column(const string& name, const string& type) {
    //     columns.push_back(Column(name, type));
    // }

    // 输出表的结构
    // void print() const {
    //     cout << "CREATE TABLE " << name << " (" << endl;
    //     for (size_t i = 0; i < columns.size(); ++i) {
    //         columns[i].print();
    //         if (i < columns.size() - 1) cout << ", ";
    //     }
    //     cout << ");" << endl;
    // }
};

class Database {
    public:
        string name;
        unordered_map<string, Table> tables;
};

extern unordered_map<string, Token> token_map;
extern string token_to_string(Token token);

struct TokenWithValue {
    Token token;
    string value;
    
    TokenWithValue(Token t, const string& v) : token(t), value(v) {}
};

bool is_number(const string& s);
bool is_string_literal(const string& s);
vector<TokenWithValue> lex(const string& input);
vector<vector<TokenWithValue>> lexfile(const string& filename);

unordered_map<string, Token> token_map = {
    {"=", Token::EQUAL},
    {">", Token::GT},
    {"<", Token::LT},
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
    {"VALUES", Token::VALUES},
    {"AND", Token::AND},
    {"OR", Token::OR}

};

string token_to_string(Token token) {
    switch (token) {
        case Token::EQUAL: return "Token::EQUAL";
        case Token::GT: return "Token::GT";
        case Token::LT: return "Token::LT";
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
        case Token::AND: return "Token::AND";
        case Token::OR: return "Token::OR";
        default: return "Unknown Token";
    }
}

// 判断字符串是否是数字
bool is_number(const string& s) {
    return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}

// 判断字符串是否是有效的字符串常量
bool is_string_literal(const string& s) {
    return !s.empty() && s.front() == '\'' && s.back() == '\'';  // 判断是否是单引号括起来的字符串
}

// 词法分析函数
vector<TokenWithValue> lex(const string& input) {
    vector<TokenWithValue> result;
    const char* ip = input.c_str();  // ip 是指向字符串的字符指针
    string token;
      // 记录是否在括号内

    while (*ip != '\0') {  // 当 ip 指向的字符不是字符串结尾时
        if (isspace(*ip)) {  // 跳过空白字符
            ++ip;
            continue;
        }

        // 如果是数字，识别为 Token::NUMBER
        if (isdigit(*ip)) {
            token.clear();
            while (isdigit(*ip) || *ip == '.') {  // 允许浮点数
                token.push_back(*ip);
                ++ip;
            }
            result.push_back(TokenWithValue(Token::NUMBER, token));
        }
        // 如果是有效的字符串常量
        else if (*ip == '\'') {  // 字符串常量以单引号开始
            token.clear();
            ++ip;  // 跳过开头的单引号
            while (*ip != '\'' && *ip != '\0') {  // 字符串结束于单引号
                token.push_back(*ip);
                ++ip;
            }
            if (*ip == '\'') {  // 跳过结尾的单引号
                ++ip;
            }
            result.push_back(TokenWithValue(Token::STRING, token));
        }
        // 处理括号符号
        else if (*ip == '(') {
            // if (!inside_paren) {
                // inside_paren = true;  // 标记进入括号
                result.push_back(TokenWithValue(Token::LPAREN, "("));
            // }
            ++ip;
        }
        else if (*ip == ')') {
            // if (inside_paren) {
                result.push_back(TokenWithValue(Token::RPAREN, ")"));
                // inside_paren = false;  // 标记离开括号
            // }
            ++ip;
        }
        // 处理逗号
        else if (*ip == ',') {
            result.push_back(TokenWithValue(Token::COMMA, ","));
            ++ip;
        }
        // 处理等号
        else if (*ip == '=') {
            result.push_back(TokenWithValue(Token::EQUAL, "="));
            ++ip;
        }
        // 处理分号
        else if (*ip == ';') {
            result.push_back(TokenWithValue(Token::SEMICOLON, ";"));
            ++ip;
        }
        // 处理星号
        else if (*ip == '*') {
            result.push_back(TokenWithValue(Token::ASTERISK, "*"));
            ++ip;
        }
        // 处理点号
        else if (*ip == '.') {
            result.push_back(TokenWithValue(Token::POINT, "."));
            ++ip;
        }
        // 检查 token 是否为已知的关键词或符号
        else if (token_map.find(string(1, *ip)) != token_map.end()) {
            token.clear();
            token.push_back(*ip);
            result.push_back(TokenWithValue(token_map[token], token));
            ++ip;
        }
        // 否则当作标识符处理
        else if (isalpha(*ip) || *ip == '_') {  // 标识符以字母或下划线开头
            token.clear();
            while (isalnum(*ip) || *ip == '_') {  // 标识符后可以包含字母、数字或下划线
                token.push_back(*ip);
                ++ip;
            }
            // 如果 token 是关键字，映射为对应的 token 类型
            if (token_map.find(token) != token_map.end()) {
                result.push_back(TokenWithValue(token_map[token], token));
            } else {
                result.push_back(TokenWithValue(Token::IDENTIFIER, token));
            }
        }
        else {
            ++ip;  // 跳过无法识别的字符
        }
    }

    return result;
}


vector<vector<TokenWithValue>> lexfile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "ERROR! Cannot open file " << filename << endl;
        return {};
    }

    vector<vector<TokenWithValue>> all_tokens;  // 二维容器，存储每一行的解析结果
    vector<TokenWithValue> current_line_tokens;  // 当前行的 token 容器
    string line;

    // 逐行读取文件
    while (getline(file, line)) {
        vector<TokenWithValue> tokens = lex(line);  // 调用 lex 函数解析每一行
        for (const auto& token : tokens) {
            current_line_tokens.push_back(token);  // 将当前 token 加入行容器
            if (token.token == Token::SEMICOLON) {  // 检测到分号
                all_tokens.push_back(current_line_tokens);  // 保存当前行
                current_line_tokens.clear();  // 开始新的行
            }
        }
    }

    // 如果最后还有未保存的 token，将其加入到 all_tokens 中
    if (!current_line_tokens.empty()) {
        all_tokens.push_back(current_line_tokens);
    }

    file.close();
    return all_tokens;
}

void select_data(vector<TokenWithValue>::const_iterator& it, vector<TokenWithValue>::const_iterator end);
unordered_map<string, Database> databases;
Database* current_database = nullptr;
vector<string> column_Name;
string symbol;

// 数据库
void create_database(const string& db_name) {
    if (databases.find(db_name) != databases.end()) {
        cerr << "Database " << db_name << " already exists.\n";
        return;
    }
    // cout << "Database " << db_name << " created.\n";
    databases[db_name] = Database{db_name};
    // cout << "Database " << db_name << " created.\n";
}

// 进入数据库
void use_database(const string& db_name) {
    if (databases.find(db_name) == databases.end()) {
        cerr << "ERROR! Database " << db_name << " does not exist.\n";
        return;
    }
    current_database = &databases[db_name];
    // cout << "Using database " << db_name << ".\n";
}

// 创建表
void create_table(const string& table_name, vector<TokenWithValue>::const_iterator& it, vector<TokenWithValue>::const_iterator end) {
    if (current_database == nullptr) {
        cerr << "ERROR! No database selected.\n";
        return;
    }

    // 创建表结构
    Table table(table_name);
    
    if (it == end || it->token != Token::LPAREN) {
        cerr << "ERROR! Expected '(' after table name.\n";
        return;
    }
    ++it; 

  
    while (it != end && it->token != Token::RPAREN) {
        if (it->token == Token::IDENTIFIER) {
            string column_name = it->value;
            ++it; 
            if (it == end || it->token != Token::IDENTIFIER) {
                cerr << "ERROR! Expected column type after column name.\n";
                return;
            }
            string column_type = it->value;
            table.columns.push_back({column_name, column_type});
            ++it; 
        }
        if (it != end && it->token == Token::COMMA) {
            ++it; 
        }
    }

    if (it == end || it->token != Token::RPAREN) {
        cerr << "ERROR! Expected ')' after column definitions.\n";
        return;
    }

    ++it;

    current_database->tables[table_name] = table;
    // cout << "Table " << table_name << " created.\n";
}

// 选择所有列
void asterisk_select(vector<TokenWithValue>::const_iterator& it, vector<TokenWithValue>::const_iterator end) {
    if (it != end && it->token == Token::FROM) {
        ++it;
        if (it != end && it->token == Token::IDENTIFIER) {
            string table_name = it->value;
            if (current_database == nullptr) {
                cerr << "ERROR! No database selected.\n";
                return;
            }
            if (current_database->tables.find(table_name) == current_database->tables.end()) {
                cerr << "ERROR! Table " << table_name << " does not exist.\n";
                return;
            }
            Table& table = current_database->tables[table_name];
            for (const auto& column : table.columns) {
                cout << column.name << ",";
            }
            cout << endl;
            for (const auto& row : table.data) {
                for (const auto& value : row) {
                    cout << value << ",";
                }
                cout << endl;
            }
        }
    }
}

// 选择条件的数据
bool is_number_where(const std::string& s) {
    try {
        std::stod(s);  // 尝试将字符串转换为数字
        return true;
    } catch (const std::invalid_argument&) {
        return false;  // 如果不能转换为数字，返回 false
    } catch (const std::out_of_range&) {
        return false;  // 如果超出范围，返回 false
    }
}

void where_select(vector<string>& column_Name, vector<TokenWithValue>::const_iterator& it, vector<TokenWithValue>::const_iterator end, Table& table, Table& table1) {
    // for (const auto& column : column_Name) {
    //     cout << column;
    //         if (column != column_Name.back()) {
    //                     cout << ",";
    //         }
    //     }
    //     cout << endl;
        ++it;
    if (it != end && it->token == Token::IDENTIFIER) {
        string column_name = it->value;
        ++it;
        // 检查操作符
        if (it != end && (it->token == Token::EQUAL || it->token == Token::GT || it->token == Token::LT)) {
            string op = it->value;
            ++it;
            if (it != end && (it->token == Token::STRING || it->token == Token::NUMBER)) {
                string value = it->value;
                // 万恶之源，修了两天5个小时，原来是这里多写了
                // try {
                //     std::stod(value);  // 尝试将字符串转换为数字
                // } catch (const std::invalid_argument&) {
                // } catch (const std::out_of_range&) {
                // }
                vector<bool> match(table.data.size());
                auto its = find_if(table.columns.begin(), table.columns.end(), [&](const Column& col) {
                        return col.name == column_name;
                    });
                for (const auto& target_row : table.data) {
                    size_t index = distance(table.columns.begin(), its);
                    if (its != table.columns.end()) {
                        if (op == "=") {
                            if (is_number_where(value)) {
                                    if (target_row[index] == value) {
                                        match[distance(table.data.begin(), find(table.data.begin(), table.data.end(), target_row))] = true;
                                }
                            } else {
                                if (target_row[index] == value) {
                                    match[distance(table.data.begin(), find(table.data.begin(), table.data.end(), target_row))] = true;
                                }
                            }
                        } else if (op == ">") {
                            if (is_number_where(value)) {
                                if (is_number_where(target_row[index])) {
                                    if (target_row[index] > value) {
                                        match[distance(table.data.begin(), find(table.data.begin(), table.data.end(), target_row))] = true;
                                    }
                                }
                            }
                        } else if (op == "<") {
                            if (is_number_where(value)) {
                                if (is_number_where(target_row[index])) {
                                    if (target_row[index] < value) {
                                        match[distance(table.data.begin(), find(table.data.begin(), table.data.end(), target_row))] = true;
                                    }
                                }
                            }
                        }
                    }
                }
                for (size_t i = 0; i < table.data.size(); ++i) {
                    if (match[i] == true) {
                        const auto& row = table.data[i];
                        vector<string> new_row;
                        for (const auto& column : column_Name) {
                            auto it = find_if(table.columns.begin(), table.columns.end(), [&](const Column& col) {
                                return col.name == column;
                            });
                            if (it != table.columns.end()) {
                                size_t index = distance(table.columns.begin(), it);
                                new_row.push_back(row[index]);
                            }
                        }
                        table1.data.push_back(new_row);
                    }
                }

                // 输出table1
                // for (const auto& row : table1.data) {
                //     for (const auto& value : row) {
                //         cout << value;
                //         if (&value != &row.back()) {
                //             cout << ",";
                //         }
                //     }
                //     cout << endl;
                // }
            }
        }

    }
}

// 选择特定一列或几列
void identifier_select(const string& column_name, vector<TokenWithValue>::const_iterator& it, vector<TokenWithValue>::const_iterator end) {
    if (it != end && it->token == Token::COMMA) {
        ++it;
        select_data(it, end);
    } else if (it != end && it->token == Token::FROM && column_Name.size() >= 1) {
        ++it;
        if (it != end && it->token == Token::IDENTIFIER) {
            string table_name = it->value;
            if (current_database == nullptr) {
                cerr << "ERROR! No database selected.\n";
                return;
            }
            if (current_database->tables.find(table_name) == current_database->tables.end()) {
                cerr << "ERROR! Table " << table_name << " does not exist.\n";
                return;
            }
            ++it;
            if (it != end && it->token == Token::WHERE) {
                Table& table = current_database->tables[table_name];
                Table table1;
                where_select(column_Name,it, end, table, table1);
                ++it;
                // for (const auto& column : column_Name) {
                //     cout << column;
                //     if (column != column_Name.back()) {
                //         cout << ",";
                //     }
                // }
                // // 输出table1
                // for (const auto& row : table1.data) {
                //     for (const auto& value : row) {
                //         cout << value;
                //         if (&value != &row.back()) {
                //             cout << ",";
                //         }
                //     }
                //     cout << endl;
                // }
                if (it != end && it->token == Token::AND) {
                    Table table2;
                    where_select(column_Name,it, end, table, table2);
                    for (const auto& column : column_Name) {
                        cout << column;
                        if (column != column_Name.back()) {
                            cout << ",";
                        }
                    }
                    cout << endl;
                    for (const auto& row1 : table1.data) {
                        for (const auto& row2 : table2.data) {
                            if (row1 == row2) {
                                for (const auto& value : row1) {
                                    cout << value;
                                    if (&value != &row1.back()) {
                                        cout << ",";
                                    }
                                }
                                cout << endl;
                            }
                        }
                    }
                } else if (it != end && it->token == Token::OR) {
                    Table table2;
                    where_select(column_Name,it, end, table, table2);
                    for (const auto& column : column_Name) {
                        cout << column;
                        if (column != column_Name.back()) {
                            cout << ",";
                        }
                    }
                    cout << endl;
                    for (const auto& row : table1.data) {
                        for (const auto& value : row) {
                            cout << value;
                            if (&value != &row.back()) {
                                cout << ",";
                            }
                        }
                        cout << endl;
                    }
                    for (const auto& row : table2.data) {
                        if (find(table1.data.begin(), table1.data.end(), row) == table1.data.end()) {
                            for (const auto& value : row) {
                                cout << value;
                                if (&value != &row.back()) {
                                    cout << ",";
                                }
                            }
                            cout << endl;
                        }
                    }
                } else {
                    for (const auto& column : column_Name) {
                        cout << column;
                        if (column != column_Name.back()) {
                            cout << ",";
                        }
                    }
                    cout << endl;
                    for (const auto& row : table1.data) {
                        for (const auto& value : row) {
                            cout << value;
                            if (&value != &row.back()) {
                                cout << ",";
                            }
                        }
                        cout << endl;
                    }
                }
            column_Name.clear();
            }else {
            Table& table = current_database->tables[table_name];
            for (const auto &column : column_Name) {
                cout << column;
                if (column != column_Name.back()) {
                    cout << ",";}
            }
            cout << endl;
            for (const auto& row : table.data) {
                for (const auto& column : column_Name) {
                    auto it = find_if(table.columns.begin(), table.columns.end(), [&](const Column& col) {
                        return col.name == column;
                    });
                    if (it != table.columns.end()) {
                        size_t index = distance(table.columns.begin(), it);
                        cout << row[index];
                        if (column != column_Name.back()) {
                            cout << ",";}
                    }
                }
                cout << endl;
            }

        }
        }
    }

}

// 选择数据
void select_data(vector<TokenWithValue>::const_iterator& it, vector<TokenWithValue>::const_iterator end) {
    if (it != end && it->token == Token::ASTERISK) {
        ++it;
        asterisk_select(it, end);
    } else if (it != end && it->token == Token::IDENTIFIER){
        string column_name = it->value;
        ++it;
        column_Name.push_back(column_name);
        identifier_select(column_name, it, end);
    }
}

// 删除数据
void asterisk_delete(vector<TokenWithValue>::const_iterator& it, vector<TokenWithValue>::const_iterator end){
        string table_name = it->value;
        if (current_database == nullptr) {
            cerr << "ERROR! No database selected.\n";
            return;
        }
        if (current_database->tables.find(table_name) == current_database->tables.end()) {
            cerr << "ERROR! Table " << table_name << " does not exist.\n";
            return;
        }
        Table& table = current_database->tables[table_name];
        table.data.clear();
        // cout << "All data from table " << table_name << " deleted.\n";
}

void identifier_delete(const string& column_name, vector<TokenWithValue>::const_iterator& it, vector<TokenWithValue>::const_iterator end){
    ++it;
    if (it != end && it->token == Token::IDENTIFIER){
        } else {
        cerr << "ERROR! Expected column name after WHERE.\n";
        return;
    }
}

void delete_data(vector<TokenWithValue>::const_iterator& it, vector<TokenWithValue>::const_iterator end){
    if (it != end && it->token == Token::DELETE){
        ++it;
        if (it != end && it->token == Token::FROM) {
            ++it;
            if (it != end && it->token == Token::IDENTIFIER) {
                 string column_name = it->value;
                ++it;
                column_Name.push_back(column_name);
                identifier_delete(column_name, it, end);
        } else {
            cerr << "ERROR! Expected FROM after DELETE.\n";
            return;
        }
    }
}
}

// 执行 SQL 命令
void execute_query(const vector<TokenWithValue>& tokens) {
    auto it = tokens.begin();
    if (it != tokens.end() && it->token == Token::CREATE){ // 创建数据库
        ++it;
        if (it != tokens.end() && it->token == Token::DATABASE) {
            ++it;
            if (it != tokens.end() && it->token == Token::IDENTIFIER) {
                string db_name = it->value;
                create_database(db_name);
            }
        } else if (it != tokens.end() && it->token == Token::TABLE) { // 创建表
            ++it;
            if (it != tokens.end() && it->token == Token::IDENTIFIER) {
                string table_name = it->value;
                create_table(table_name, ++it, tokens.end());
            }
        }
    } else if (it != tokens.end() && it->token == Token::USE) {
        ++it;
        if (it != tokens.end() && it->token == Token::DATABASE) {
            ++it;
            if (it != tokens.end() && it->token == Token::IDENTIFIER) {
                string db_name = it->value;
                use_database(db_name);}
        }
    } else if (it != tokens.end() && it->token == Token::DROP){ // 删除表
        ++it;
        if (it != tokens.end() && it->token == Token::TABLE) {
            ++it;
            if (it != tokens.end() && it->token == Token::IDENTIFIER) {
                string table_name = it->value;
                if (current_database == nullptr) {
                    cerr << "ERROR! No database selected.\n";
                    return;
                }
                if (current_database->tables.find(table_name) == current_database->tables.end()) {
                    cerr << "ERROR! Table " << table_name << " does not exist.\n";
                    return;
                }
                current_database->tables.erase(table_name);
                // cout << "Table " << table_name << " dropped.\n";
            }
        }
    } else if (it != tokens.end() && it->token == Token::INSERT) { // 插入数据
        ++it;
        if (it != tokens.end() && it->token == Token::INTO) {
            ++it;
            if (it != tokens.end() && it->token == Token::IDENTIFIER) {
                string table_name = it->value;
                if (current_database == nullptr) {
                    cerr << "ERROR! No database selected.\n";
                    return;
                }
                if (current_database->tables.find(table_name) == current_database->tables.end()) {
                    cerr << "ERROR! Table " << table_name << " does not exist.\n";
                    return;
                }
                Table& table = current_database->tables[table_name];
                ++it;
                if (it != tokens.end() && it->token == Token::VALUES) {
                    ++it;
                    if (it != tokens.end() && it->token == Token::LPAREN) {
                        ++it;
                        vector<string> values;
                        while (it != tokens.end() && it->token != Token::RPAREN) {
                            if (it->token == Token::STRING || it->token == Token::NUMBER) {
                                values.push_back(it->value);
                                ++it;
                            }
                            if (it != tokens.end() && it->token == Token::COMMA) {
                                ++it;
                            }
                        }
                        if (it != tokens.end() && it->token == Token::RPAREN) {
                            if (values.size() != table.columns.size()) {
                                cerr << "ERROR! Number of values does not match number of columns.\n";
                                return;
                            }
                            table.data.push_back(values);
                            // cout << "Inserted into " << table_name << ": ";
                            // for (const auto& value : values) {
                            //     cout << value << " ";
                            // }
                            // cout << endl;
                        }
                    }
                }
            }
        }
    } 
    else if (it != tokens.end() && it->token == Token::SELECT) { 
        ++it;
        select_data(it, tokens.end());
    } else if (it != tokens.end() && it->token == Token::UPDATE) {
        ++it;
        update_data(it, tokens.end());
    } else if (it != tokens.end() && it->token == Token::DELETE) {
        ++it;
        delete_data(it, tokens.end());
    } else {
        cerr << "ERROR! Unknown command.\n";
    }
}

int main() {
    vector<vector<TokenWithValue>> lex_output = lexfile("input.sql");
    // for (const auto& line_tokens : lex_output) {
    //     for (const auto& token : line_tokens) {
    //         cout << token_to_string(token.token) << " ";
    //     }
    //     cout << endl;
    // }  // 输出词法分析结果

    for (const auto& line_tokens : lex_output) {
        execute_query(line_tokens);
    } // 执行 SQL 命令
    // for (const auto& table : current_database->tables) {
    //     cout << "Table " << table.first << ":\n";
    //     for (const auto& column : table.second.columns) {
    //         cout << column.name << "," ;
    //     }
    //     cout << endl;
    //     for (const auto& row : table.second.data) {
    //         for (const auto& value : row) {
    //             cout << value << ",";
    //         }
    //         cout << endl;
    //     }
    // }
    // for (const auto& db : databases) {
    //     cout << "Database " << db.first << ":\n";
    //     for (const auto& table : db.second.tables) {
    //         cout << "Table " << table.first << ":\n";
    //         for (const auto& column : table.second.columns) {
    //             cout << column.name << "," ;
    //         }
    //         for (const auto& row : table.second.data) {
    //             for (const auto& value : row) {
    //                 cout << value << ",";
    //             }
    //             cout << endl;
    //         }
    //     }
    // }  // 输出数据库和表的信息
    
    return 0;
}
