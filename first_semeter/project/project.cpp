#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cctype>
#include <algorithm>
#include <stack>
#include <cmath>
#include <stdexcept>
#include <iomanip>
#include <map>
using namespace std;

int colnum = 0;
int select_num = 0;
ofstream out;

void initialize_output_file(string filename) {
    out.open(filename, ios::out | ios::trunc); // 以截断模式打开，清空文件
    if (!out.is_open()) {
        cerr << "ERROR! Unable to open output.csv for writing." << endl;
    }
}

void close_output_file() {
    if (out.is_open()) {
        out.close();
    }
}

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
    INNER,
    JOIN,
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
    OR,
    PLUS, // 加法
    MINUS, // 减法 
    DIVIDE, // 除法
    MULTIPLY, // 乘法
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
void select_data(vector<TokenWithValue>::const_iterator& it, vector<TokenWithValue>::const_iterator end);
unordered_map<string, Database> databases;
Database* current_database = nullptr;
vector<string> column_Name;
string symbol;

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
    {"OR", Token::OR},
    {"INNER", Token::INNER},
    {"JOIN", Token::JOIN},
    {"+", Token::PLUS},
    {"-", Token::MINUS},
    {"/", Token::DIVIDE},
    {"*", Token::MULTIPLY}
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
        case Token::INNER: return "Token::INNER";
        case Token::JOIN: return "Token::JOIN";
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
        case Token::PLUS: return "Token::PLUS";
        case Token::MINUS: return "Token::MINUS";
        case Token::DIVIDE: return "Token::DIVIDE";
        case Token::MULTIPLY: return "Token::MULTIPLY";
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

// 数据库
void create_database(const string& db_name) {
    if (databases.find(db_name) != databases.end()) {
        cerr << "Database " << db_name << " already exists." << "At column " << colnum << endl;
        return;
    }
    // cout << "Database " << db_name << " created.\n";
    databases[db_name] = Database{db_name};
    // cout << "Database " << db_name << " created.\n";
}

// 进入数据库
void use_database(const string& db_name) {
    if (databases.find(db_name) == databases.end()) {
        cerr << "ERROR! Database " << db_name << " does not exist." << "At column " << colnum << endl;
        return;
    }
    current_database = &databases[db_name];
    // cout << "Using database " << db_name << ".\n";
}

// 创建表
void create_table(const string& table_name, vector<TokenWithValue>::const_iterator& it, vector<TokenWithValue>::const_iterator end) {
    if (current_database == nullptr) {
        cerr << "ERROR! No database selected." << "At column " << colnum << endl;
        return;
    }

    // 创建表结构
    Table table(table_name);
    
    if (it == end || it->token != Token::LPAREN) {
        cerr << "ERROR! Expected '(' after table name." << "At column " << colnum << endl;
        return;
    }
    ++it; 

  
    while (it != end && it->token != Token::RPAREN) {
        if (it->token == Token::IDENTIFIER) {
            string column_name = it->value;
            ++it; 
            if (it == end || it->token != Token::IDENTIFIER) {
                cerr << "ERROR! Expected column type after column name." << "At column " << colnum << endl;
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
        cerr << "ERROR! Expected ')' after column definitions. " << "At column " << colnum << endl;
        return;
    }

    ++it;

    current_database->tables[table_name] = table;
    // cout << "Table " << table_name << " created.\n";
}

// 选择数据
void asterisk_select(vector<TokenWithValue>::const_iterator& it, vector<TokenWithValue>::const_iterator end) {
    if (it != end && it->token == Token::FROM) {
        ++it;
        if (it != end && it->token == Token::IDENTIFIER) {
            string table_name = it->value;
            if (current_database == nullptr) {
                cerr << "ERROR! No database selected." << "At column " << colnum << endl;
                return;
            }
            if (current_database->tables.find(table_name) == current_database->tables.end()) {
                cerr << "ERROR! Table " << table_name << " does not exist." << "At column " << colnum << endl;
                return;
            }
            Table& table = current_database->tables[table_name];
            if (select_num !=0 ){
                out << "---" << endl;
            }
            ++select_num;
            for (const auto& column : table.columns) {
                out << column.name;
                if (&column != &table.columns.back()) {
                    out << ",";
                }
            }
            out << endl;
            for (const auto& row : table.data) {
                for (const auto& value : row) {
                    out << value;
                    if (&value != &row.back()) {
                        out << ",";
                    }
                }
                out << endl;
            }
        } else {
            cerr << "ERROR! Expected table name after FROM." << "At column " << colnum << endl;
            return;
        }
    } else {
        cerr << "ERROR! Expected FROM after *." << "At column " << colnum << endl;
        return;
    }
}

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
                // 万恶之源
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
            } else {
                cerr << "ERROR! Expected value after operator." << "At column " << colnum << endl;
                return;
            }
        } else {
            cerr << "ERROR! Expected operator after column name." << "At column " << colnum << endl;
            return;
        }
    } else {
        cerr << "ERROR! Expected column name after WHERE." << "At column " << colnum << endl;
        return;
    }
}

void inner_helper(Table& table1, Table& table2, const string& column1_name1, const string& column1_name2, const string& column2_name1, const string& column2_name2) {
    // 查找列索引
    int col1_index1 = -1, col1_index2 = -1, col2_index1 = -1, col2_index2 = -1;

    for (size_t i = 0; i < table1.columns.size(); ++i) {
        if (table1.columns[i].name == column1_name1) col1_index1 = i;
        if (table1.columns[i].name == column1_name2) col1_index2 = i;
    }

    for (size_t i = 0; i < table2.columns.size(); ++i) {
        if (table2.columns[i].name == column2_name1) col2_index1 = i;
        if (table2.columns[i].name == column2_name2) col2_index2 = i;
    }

    // 检查列是否找到
    if (col1_index1 == -1 || col1_index2 == -1 || col2_index1 == -1 || col2_index2 == -1) {
        cerr << "One or more columns not found!" << endl;
        return;
    }

    // 构建结果
    vector<vector<string>> result;

    // 建立表1的 column1_name2 -> column1_name1 映射
    unordered_map<string, string> table1_map;
    for (const auto& row : table1.data) {
        table1_map[row[col1_index2]] = row[col1_index1];  // 使用 column1_name2 作为 key, column1_name1 作为 value
    }

    // 遍历表2数据，匹配并构造结果
    for (const auto& row : table2.data) {
        const string& key = row[col2_index2];  // 获取 column2_name2 的值作为 key
        if (table1_map.find(key) != table1_map.end()) {
            vector<string> new_row = {table1_map[key], row[col2_index1]};  // 匹配后输出 column1_name1 和 column2_name1 的值
            result.push_back(new_row);
        } else {
            cerr << "No match found for " << key << endl;
        }
    }

    if (result.empty()) {
        cerr << "No result found." << endl;
        return;
    }

    if (select_num !=0 ){
        out << "---" << endl;
    }
    ++select_num;
    // 输出结果
    out << column1_name1 << ", " << column2_name1 << endl;
    for (const auto& row : result) {
        for (size_t i = 0; i < row.size(); ++i) {
            out << row[i];
            if (i < row.size() - 1) out << ", ";
        }
        out << endl;
    }
}

void innerjoin(vector<TokenWithValue>::const_iterator& it, vector<TokenWithValue>::const_iterator end, string& table_name1, string& column1_name1, string& table_name2, string& column2_name1, Table& table1, Table& table2) {
    if (it != end && it->token == Token::IDENTIFIER && it->value == table_name1) {
        ++it;
        if (it != end && it->token == Token::POINT) {
            ++it;
            if (it != end && it->token == Token::IDENTIFIER) {
                string column1_name2 = it->value;
                ++it;
                if (it != end && it->token == Token::EQUAL) {
                    ++it;
                    if (it != end && it->token == Token::IDENTIFIER && it->value == table_name2) {
                        ++it;
                        if (it != end && it->token == Token::POINT) {
                            ++it;
                            if (it != end && it->token == Token::IDENTIFIER) {
                                string column2_name2 = it->value;
                                inner_helper(table1, table2, column1_name1, column1_name2, column2_name1, column2_name2);
                            } else {
                                cerr << "ERROR! Expected column name after POINT. " << "At column " << colnum << endl;
                                return;
                            }
                        } else {
                            cerr << "ERROR! Expected POINT after table name. " << "At column " << colnum << endl;
                            return;
                        }
                    } else if (it != end && it->token == Token::IDENTIFIER && it->value != table_name2) {
                        cerr << "ERROR! The table name does not equal to the second table name. " << "At column " << colnum << endl;
                        return;
                    } else {
                        cerr << "ERROR! Expected table name after EQUAL. " << "At column " << colnum << endl;
                        return;
                    }
                } else {
                    cerr << "ERROR! Expected EQUAL after column name." << "At column " << colnum << endl;
                    return;
                }
            } else {
                cerr << "ERROR! Expected column name after POINT." << "At column " << colnum << endl;
                return;
            }
        } else {
            cerr << "ERROR! Expected POINT after table name." << "At column " << colnum << endl;
            return;
        }
    } else if (it != end && it->token == Token::IDENTIFIER && it->value != table_name1) {
        cerr << "ERROR! The table name does not equal to the first table name." << "At column " << colnum << endl;
        return;
    } else {
        cerr << "ERROR! Expected table name after ON." << "At column " << colnum << endl;
        return;}
}

void INNERJOIN(vector<TokenWithValue>::const_iterator& it, vector<TokenWithValue>::const_iterator end) {
    --it;
    if (it != end && it->token == Token::IDENTIFIER) {
        string table_name1 = it->value;
        Table& table1 = current_database->tables[table_name1];
        ++it;
        if (it != end && it->token == Token::POINT) {
            ++it;
            if (it != end && it->token == Token::IDENTIFIER) {
                string column1_name1 = it->value;
                ++it;
                if (it != end && it->token == Token::COMMA) {
                    ++it;
                    if (it != end && it->token == Token::IDENTIFIER){
                        string table_name2 = it->value;
                        Table& table2 = current_database->tables[table_name2];
                        ++it;
                        if (it != end && it->token == Token::POINT) {
                            ++it;
                            if (it !=end && it->token == Token::IDENTIFIER) {
                                string column2_name1 = it->value;
                                ++it;
                                if (it != end && it->token == Token::FROM){
                                    ++it;
                                    if (it != end && it->token == Token::IDENTIFIER) {
                                        ++it;
                                        if (it != end && it->token == Token::INNER) {
                                            ++it;
                                            if (it != end && it->token == Token::JOIN) {
                                                ++it;
                                                if (it != end && it->token == Token::IDENTIFIER) {
                                                    ++it;
                                                    if (it != end && it->token == Token::ON) {
                                                        ++it;
                                                        innerjoin(it, end, table_name1, column1_name1, table_name2, column2_name1, table1, table2);
                                                    } else {
                                                        cerr << "ERROR! Expected ON after JOIN. " << "At column " << colnum << endl;
                                                        return;
                                                    }
                                                }
                                            } else {
                                                cerr << "ERROR! Expected JOIN after INNER. " << "At column " << colnum << endl;
                                                return;
                                            }
                                        } else {
                                            cerr << "ERROR! Expected INNER after FROM. " << "At column " << colnum << endl;
                                            return;
                                        }
                                    } else {
                                        cerr << "ERROR! Expected table name after FROM. " << "At column " << colnum << endl;
                                        return;
                                    }
                                } else {
                                    cerr << "ERROR! Expected FROM after column name. " << "At column " << colnum << endl;
                                    return;
                                }
                            } else {
                                cerr << "ERROR! Expected column name after POINT. " << "At column " << colnum << endl;
                                return;
                            }
                        } else {
                            cerr << "ERROR! Expected POINT after table name. " << "At column " << colnum << endl;
                            return;
                        }
                    } else {
                        cerr << "ERROR! Expected table name after COMMA. " << "At column " << colnum << endl;
                        return;
                    }
                } else {
                    cerr << "ERROR! Expected COMMA after column name. " << "At column " << colnum << endl;
                    return;
                }
            } else {
                cerr << "ERROR! Expected column name after POINT. " << "At column " << colnum << endl;
                return;
            }
        } else {
            cerr << "ERROR! Expected POINT after table name. " << "At column " << colnum << endl;
            return;
        }
    } else {
        cerr << "ERROR! Expected table name after POINT. " << "At column " << colnum << endl;
        return;}
}

void identifier_select(const string& column_name, vector<TokenWithValue>::const_iterator& it, vector<TokenWithValue>::const_iterator end) {
    if (it != end && it->token == Token::POINT) {
        INNERJOIN(it ,end);
    }
    if (it != end && it->token == Token::COMMA) {
        ++it;
        select_data(it, end);
    } else if (it != end && it->token == Token::FROM && column_Name.size() >= 1) {
        ++it;
        if (it != end && it->token == Token::IDENTIFIER) {
            string table_name = it->value;
            if (current_database == nullptr) {
                cerr << "ERROR! No database selected." << "At column " << colnum << endl;
                return;
            }
            if (current_database->tables.find(table_name) == current_database->tables.end()) {
                cerr << "ERROR! Table " << table_name << " does not exist." << "At column " << colnum << endl;
                return;
            }
            ++it;
            if (it != end && it->token == Token::WHERE) {
                Table& table = current_database->tables[table_name];
                Table table1;
                where_select(column_Name,it, end, table, table1);
                ++it;
                if (it != end && it->token == Token::AND) {
                    Table table2;
                    where_select(column_Name,it, end, table, table2);
                    for (const auto &column : column_Name) {
                        auto it = find_if(table.columns.begin(), table.columns.end(), [&](const Column& col) {
                            return col.name == column;
                        });
                        if (it == table.columns.end()) {
                            cerr << "ERROR! Column " << column << " does not exist in table " << table_name << "." << "At column " << colnum << endl;
                            return;
                        }
                        if (select_num !=0 ){
                            out << "---" << endl;
                        }
                        ++select_num;
                        out << column;
                        if (column != column_Name.back()) {
                            out << ",";
                        }
                    }
                    out << endl;
                    for (const auto& row1 : table1.data) {
                        for (const auto& row2 : table2.data) {
                            if (row1 == row2) {
                                for (const auto& value : row1) {
                                    out << value;
                                    if (&value != &row1.back()) {
                                        out << ",";
                                    }
                                }
                                out << endl;
                            }
                        }
                    }
                } else if (it != end && it->token == Token::OR) {
                    Table table2;
                    where_select(column_Name,it, end, table, table2);
                    for (const auto &column : column_Name) {
                        auto it = find_if(table.columns.begin(), table.columns.end(), [&](const Column& col) {
                            return col.name == column;
                        });
                        if (it == table.columns.end()) {
                            cerr << "ERROR! Column " << column << " does not exist in table " << table_name << "." << "At column " << colnum << endl;
                            return;
                        }
                        if (select_num !=0 ){
                        out << "---" << endl;
                        }
                        ++select_num;
                        out << column;
                        if (column != column_Name.back()) {
                            out << ",";
                        }
                    }
                    out << endl;
                    for (const auto& row : table1.data) {
                        for (const auto& value : row) {
                            out << value;
                            if (&value != &row.back()) {
                                out << ",";
                            }
                        }
                        out << endl;
                    }
                    for (const auto& row : table2.data) {
                        if (find(table1.data.begin(), table1.data.end(), row) == table1.data.end()) {
                            for (const auto& value : row) {
                                out << value;
                                if (&value != &row.back()) {
                                    out << ",";
                                }
                            }
                            out << endl;
                        }
                    }
                } else {
                    for (const auto &column : column_Name) {
                        auto it = find_if(table.columns.begin(), table.columns.end(), [&](const Column& col) {
                            return col.name == column;
                        });
                        if (it == table.columns.end()) {
                            cerr << "ERROR! Column " << column << " does not exist in table " << table_name << "." << "At column " << colnum << endl;
                            return;
                        }
                        if (select_num !=0 ){
                            out << "---" << endl;
                        }
                        ++select_num;
                        out << column;
                        if (column != column_Name.back()) {
                            out << ",";
                        }
                    }
                    out << endl;
                    for (const auto& row : table1.data) {
                        for (const auto& value : row) {
                            out << value;
                            if (&value != &row.back()) {
                                out << ",";
                            }
                        }
                        out << endl;
                    }
                }
            column_Name.clear();
            } else {
            Table& table = current_database->tables[table_name];
            for (const auto &column : column_Name) {
                auto it = find_if(table.columns.begin(), table.columns.end(), [&](const Column& col) {
                    return col.name == column;
                });
                if (it == table.columns.end()) {
                    cerr << "ERROR! Column " << column << " does not exist in table " << table_name << "." << "At column " << colnum << endl;
                    return;
                }
                if (select_num !=0 ){
                    out << "---" << endl;
                }
                ++select_num;
                out << column;
                if (column != column_Name.back()) {
                    out << ",";
                }
            }
            out << endl;
            for (const auto& row : table.data) {
                for (const auto& column : column_Name) {
                    auto it = find_if(table.columns.begin(), table.columns.end(), [&](const Column& col) {
                        return col.name == column;
                    });
                    if (it != table.columns.end()) {
                        size_t index = distance(table.columns.begin(), it);
                        out << row[index];
                        if (column != column_Name.back()) {
                            out << ",";}
                    }
                }
                out << endl;
            }
            column_Name.clear();
        }
        } else {
            cerr << "ERROR! Expected WHERE or ; after column name." << "At column " << colnum << endl;
            return;
        } 
    }
}

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
            cerr << "ERROR! No database selected." << "At column " << colnum << endl;
            return;
        }
        if (current_database->tables.find(table_name) == current_database->tables.end()) {
            cerr << "ERROR! Table " << table_name << " does not exist." << "At column " << colnum << endl;
            return;
        }
        Table& table = current_database->tables[table_name];
        table.data.clear();
        // cout << "All data from table " << table_name << " deleted.\n";
}

void where_delete(vector<TokenWithValue>::const_iterator& it, vector<TokenWithValue>::const_iterator end, Table& table, Table& table1) {
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
                for (size_t i = table.data.size(); i-- > 0;) {
                    if (match[i] == true) {
                        table.data.erase(table.data.begin() + i);
                    }
                }
            } else {
                cerr << "ERROR! Expected value after operator." << "At column " << colnum << endl;
                return;
            }
        } else {
            cerr << "ERROR! Expected operator after column name." << "At column " << colnum << endl;
            return;
        }
    } else {
        cerr << "ERROR! Expected column name after WHERE." << "At column " << colnum << endl;
        return;
    }
}

void identifier_delete(const string& table_name, vector<TokenWithValue>::const_iterator& it, vector<TokenWithValue>::const_iterator end){
    ++it;
    if (it != end && it->token == Token::WHERE){
        Table& table = current_database->tables[table_name];
        Table table1;
        where_delete(it, end, table, table1);
        ++it;
        } else if (it != end && it->token == Token::SEMICOLON) {
            --it;
            asterisk_delete(it, end);
        } else {
            cerr << "ERROR! Expected WHERE or ; after column name." << "At column " << colnum << endl;
            return;
        }
}

void delete_data(vector<TokenWithValue>::const_iterator& it, vector<TokenWithValue>::const_iterator end){
        if (it != end && it->token == Token::FROM) {
            ++it;
            if (it != end && it->token == Token::IDENTIFIER) {
                 string table_name = it->value;
                identifier_delete(table_name, it, end);
        } else {
            cerr << "ERROR! Expected FROM after DELETE." << "At column " << colnum << endl;
            return;
        }
    }
}

// 更新数据
double evaluate(const string& expression, const map<string, double>& variables) {
    stack<double> values;
    stack<char> operators;

    // 判断运算符优先级
    auto precedence = [](char op) {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        return 0;
    };

    // 执行运算
    auto applyOp = [](double a, double b, char op) {
        switch (op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': if (b == 0) throw runtime_error("Division by zero");
                      return a / b;
            default: throw runtime_error("Unsupported operator");
        }
    };

    // 解析变量或数字
    auto resolveVariable = [&](const string& token) -> double {
        if (isdigit(token[0]) || (token[0] == '-' && token.size() > 1)) {
            return stod(token); // 是数字，直接转换
        }
        auto it = variables.find(token);
        if (it != variables.end()) {
            return it->second; // 从变量表中获取值
        }
        throw runtime_error("Undefined variable: " + token);
    };

    stringstream ss(expression);
    string token;

    while (ss >> token) {
        if (isdigit(token[0]) || (token[0] == '-' && token.size() > 1)) {
            values.push(stod(token)); // 直接压入数字
        } else if (isalpha(token[0])) {
            values.push(resolveVariable(token)); // 是变量，解析其值
        } else if (token == "(") {
            operators.push('('); // 左括号直接压入栈
        } else if (token == ")") {
            // 计算括号内的表达式
            while (!operators.empty() && operators.top() != '(') {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = operators.top(); operators.pop();
                values.push(applyOp(a, b, op));
            }
            operators.pop(); // 弹出 '('
        } else {
            // 处理运算符
            while (!operators.empty() && precedence(operators.top()) >= precedence(token[0])) {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = operators.top(); operators.pop();
                values.push(applyOp(a, b, op));
            }
            operators.push(token[0]);
        }
    }

    // 处理剩下的运算符
    while (!operators.empty()) {
        double b = values.top(); values.pop();
        double a = values.top(); values.pop();
        char op = operators.top(); operators.pop();
        values.push(applyOp(a, b, op));
    }

    return values.top(); // 返回最终的结果
}

int getDecimalPlaces(const string& s) {
    size_t pos = s.find('.');
    if (pos == string::npos) {
        return 0;
    }
    return s.size() - pos - 1;
}

void update_helper(Table& table, const string& column_name, const string& expression, const string& condition_column, const string& op, const string& value, int digit_or_identifier) {
    vector<bool> match(table.data.size(), false);
    if (digit_or_identifier == 1) {
        auto its = find_if(table.columns.begin(), table.columns.end(), [&](const Column& col) {
            return col.name == condition_column;
        });
        if (its == table.columns.end()) {
            throw std::runtime_error("Condition column not found.");
        }
        size_t condition_index = distance(table.columns.begin(), its);

        for (size_t i = 0; i < table.data.size(); ++i) {
            const auto& target_row = table.data[i];
            const auto& target_value = target_row[condition_index];

            if (op == "=" && target_value == value) {
                match[i] = true;
            } else if (op == ">" && is_number_where(value) && is_number_where(target_value)) {
                if (std::stod(target_value) - std::stod(value) > 1e-9) {
                    match[i] = true;
                }
            } else if (op == "<" && is_number_where(value) && is_number_where(target_value)) {
                if (std::stod(target_value) - std::stod(value) < 1e-9) {
                    match[i] = true;
                }
            }
        }
        for (size_t i = 0; i < table.data.size(); ++i) {
            if (match[i]) {
                    auto col_it = find_if(table.columns.begin(), table.columns.end(), [&](const Column& col) {
                        return col.name == column_name;
                    });
                    if (col_it != table.columns.end()) {
                        size_t index = distance(table.columns.begin(), col_it);
                        table.data[i][index] = expression;
                        // cout << "Updated"<< endl;
                }
            }
        }
    }  else if (digit_or_identifier > 1) {
        auto its = find_if(table.columns.begin(), table.columns.end(), [&](const Column& col) {
            return col.name == condition_column;
        });
        if (its == table.columns.end()) {
            throw std::runtime_error("Condition column not found.");
        }
        size_t condition_index = distance(table.columns.begin(), its);

        for (size_t i = 0; i < table.data.size(); ++i) {
            const auto& target_row = table.data[i];
            const auto& target_value = target_row[condition_index];

            if (op == "=" && target_value == value) {
                match[i] = true;
            } else if (op == ">" && is_number_where(value) && is_number_where(target_value)) {
                if (std::stod(target_value) > std::stod(value)) {
                    match[i] = true;
                }
            } else if (op == "<" && is_number_where(value) && is_number_where(target_value)) {
                if (std::stod(target_value) < std::stod(value)) {
                    match[i] = true;
                }
            }
        }

        // 替换表达式中的列名为 'x'
        string replaced_expression = expression;
        size_t pos = 0;
        while ((pos = replaced_expression.find(column_name, pos)) != string::npos) {
            replaced_expression.replace(pos, column_name.length(), "x");
            pos += column_name.length();  // 移动到下一个位置
        }

        for (size_t i = 0; i < table.data.size(); ++i) {
            if (match[i]) {
                auto col_it = find_if(table.columns.begin(), table.columns.end(), [&](const Column& col) {
                    return col.name == column_name;
                });
                if (col_it != table.columns.end()) {
                    size_t index = distance(table.columns.begin(), col_it);
                    int decimal_places = getDecimalPlaces(table.data[i][index]);

                    // 构造变量表
                    map<string, double> variables;
                    variables["x"] = std::stod(table.data[i][index]);

                    // 计算结果并格式化
                    try {
                        std::ostringstream oss;
                        oss << std::fixed << std::setprecision(decimal_places) << evaluate(replaced_expression, variables);
                        table.data[i][index] = oss.str();
                    } catch (const std::runtime_error& e) {
                        std::cerr << "Error evaluating expression: " << e.what() << std::endl;
                    }
                }
            }
        }
    } else {
        cerr << "ERROR! Expected digit or identifier after EQUAL." << "At column " << colnum << endl;
    }
}

void update_data(vector<TokenWithValue>::const_iterator& it, vector<TokenWithValue>::const_iterator end) {
    vector<string> expression_list;
    vector<string> column_name_list;
    string expression;
    int digit_or_identifier = 0;
    vector<int> digit_or_identifier_list;
    int size = 0;  // 初始化 size，避免未定义的行为

    if (it != end && it->token == Token::IDENTIFIER) {
        Table& table = current_database->tables[it->value];
        ++it;
        if (it != end && it->token == Token::SET) {
            ++it;
            while (it != end && it->token != Token::WHERE) {
                if (it != end && it->token == Token::IDENTIFIER) {
                    column_name_list.push_back(it->value);  // 记录列名
                    ++it;
                    if (it != end && it->token == Token::EQUAL) {
                        ++it;
                        expression.clear();  // 每次处理新表达式时清空
                        while (it != end && it->token != Token::COMMA && it->token != Token::WHERE) {
                            if (digit_or_identifier >= 1) {
                                expression += " ";  // 添加空格分隔符
                            }
                            expression += it->value;  // 拼接表达式
                            ++digit_or_identifier;
                            ++it;
                        }
                        expression_list.push_back(expression);  // 将表达式加入列表
                        ++size;  // 增加已处理表达式数量
                        expression.clear();  // 清空 expression 以处理下一个
                        digit_or_identifier_list.push_back(digit_or_identifier);  // 记录当前表达式中数字和标识符的数量
                        digit_or_identifier = 0;  // 重置拼接标志
                    }
                }
                if (it != end && it->token == Token::COMMA) {
                    ++it;  // 跳过逗号
                }
            }
            // 解析 WHERE 子句
            if (it != end && it->token == Token::WHERE) {
                ++it;
                if (it != end && it->token == Token::IDENTIFIER) {
                    string condition_column = it->value;
                    ++it;
                    if (it != end && (it->token == Token::EQUAL || it->token == Token::GT || it->token == Token::LT)) {
                        string symbol = it->value;
                        ++it;
                        if (it != end && (it->token == Token::IDENTIFIER || it->token == Token::NUMBER)) {
                            string condition_value = it->value;
                            // 调用更新函数
                            for (int i = 0; i < size; ++i) {
                                update_helper(table, column_name_list[i], expression_list[i], condition_column, symbol, condition_value, digit_or_identifier_list[i]);
                            }
                        } else {
                            cerr << "ERROR! Expected value after symbol." << "At column " << colnum << endl;
                            return;
                        }
                    } else {
                        cerr << "ERROR! Expected symbol after column name." << "At column " << colnum << endl;
                        return;
                    }
                } else {
                    cerr << "ERROR! Expected column name after WHERE." << "At column " << colnum << endl;
                    return;
                }
            } else {
                cerr << "ERROR! Expected WHERE after expressions." << "At column " << colnum << endl;
                return;
            }
        } else {
            cerr << "ERROR! Expected SET after table name." << "At column " << colnum << endl;
            return;
        }
    } else {
        cerr << "ERROR! Expected table name after UPDATE." << "At column " << colnum << endl;
        return;
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
                    cerr << "ERROR! No database selected." << "At column " << colnum << endl;
                    return;
                }
                if (current_database->tables.find(table_name) == current_database->tables.end()) {
                    cerr << "ERROR! Table " << table_name << " does not exist." << "At column " << colnum << endl;
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
                    cerr << "ERROR! No database selected." << "At column " << colnum << endl;
                    return;
                }
                if (current_database->tables.find(table_name) == current_database->tables.end()) {
                    cerr << "ERROR! Table " << table_name << " does not exist." << "At column " << colnum << endl;
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
                                cerr << "ERROR! Number of values does not match number of columns." << "At column " << colnum << endl;
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
        cerr << "ERROR! Unknown command." << "At column " << colnum << endl;
    }
}

int main() {
    string input, output;
    input = "input.sql";
    output = "output.csv";
    vector<vector<TokenWithValue>> lex_output = lexfile(input);
    initialize_output_file(output);

    // for (const auto& line_tokens : lex_output) {
    //     for (const auto& token : line_tokens) {
    //         cout << token_to_string(token.token) << " ";
    //     }
    //     cout << endl;
    // }  // 输出词法分析结果

    for (const auto& line_tokens : lex_output) {
        ++colnum;
        execute_query(line_tokens);
    } // 执行 SQL 命令

    close_output_file();
    
    return 0;
}
