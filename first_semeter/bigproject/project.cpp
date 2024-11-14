#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

enum class Token {
    IDENTIFIER,  // 列名
    STRING,      // 字符串
    NUMBER,      // 数字
    EQUAL,       // =
    GT,          // >
    LT           // <
};

struct TokenWithValue {
    Token token;
    string value;
};

struct Column {
    string name;
    // 其他列属性
};

struct Table {
    vector<Column> columns;  // 表的列
    vector<vector<string>> data;  // 表的数据
};

// 判断字符串是否是数字
bool is_number(const string& s) {
    try {
        stod(s);  // 尝试将字符串转换为数字
        return true;
    } catch (const invalid_argument&) {
        return false;  // 如果无法转换为数字，返回 false
    } catch (const out_of_range&) {
        return false;  // 如果数字超出范围，返回 false
    }
}

void where_select(vector<string>& column_Name, vector<TokenWithValue>::const_iterator& it, vector<TokenWithValue>::const_iterator end, Table& table) {
    if (it != end && it->token == Token::IDENTIFIER) {
        string column_name = it->value;
        ++it;

        // 检查操作符
        if (it != end && it->token == Token::EQUAL) {
            ++it;
            if (it != end && (it->token == Token::STRING || it->token == Token::NUMBER)) {
                string value = it->value;
                ++it;

                for (size_t i = 0; i < table.columns.size(); ++i) {
                    if (column_name == table.columns[i].name) {  // 找到目标列
                        for (const auto& row : table.data) {
                            bool match = false;

                            // 进行比较，考虑数字与字符串的情况
                            if (is_number(value)) {
                                // 如果 value 是数字
                                double compare_val = stod(value);
                                double row_val = stod(row[i]);  // 将行中的数据转为数字进行比较
                                match = (row_val == compare_val);
                            } else {
                                // 如果 value 是字符串
                                match = (row[i] == value);
                            }
                            // 如果匹配，输出对应的列
                            if (match) {
                                for (size_t j = 0; j < column_Name.size(); ++j) {
                                    for (size_t k = 0; k < table.columns.size(); ++k) {
                                        if (column_Name[j] == table.columns[k].name) {
                                            cout << row[k];
                                            if (j < column_Name.size() - 1) {
                                                cout << ",";
                                            }
                                        }
                                    }
                                }
                                cout << endl;
                            }
                        }
                    }
                }
            }
        }
    }
}

int main() {
    // 示例数据
    Table table;
    table.columns = {{"ID"}, {"Name"}, {"GPA"}};
    table.data = {{"1000", "Jay Chou", "3.0"},
                  {"1001", "Taylor Swift", "3.2"},
                  {"1002", "Eminem", "3.5"}};

    // 查询列名
    vector<string> column_Name = {"ID", "Name"};

    // 模拟 SQL 查询中的 token，SELECT ID, Name FROM student WHERE GPA = 3.0
    vector<TokenWithValue> tokens = {
        {Token::IDENTIFIER, "GPA"},
        {Token::EQUAL, "="},
        {Token::NUMBER, "3.0"}
    };

    // 调用 where_select 进行查询
    vector<TokenWithValue>::const_iterator it = tokens.begin();
    where_select(column_Name, it, tokens.end(), table);

    // 模拟查询：SELECT ID, Name FROM student WHERE Name = 'Taylor Swift'
    vector<string> column_Name_2 = {"ID", "Name"};
    vector<TokenWithValue> tokens_2 = {
        {Token::IDENTIFIER, "Name"},
        {Token::EQUAL, "="},
        {Token::STRING, "Taylor Swift"}
    };

    it = tokens_2.begin();
    where_select(column_Name_2, it, tokens_2.end(), table);

    return 0;
}

