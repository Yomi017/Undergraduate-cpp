#include <iostream>
#include <stack>
#include <map>
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <string>

using namespace std;

bool evaluateBool(const string& expression, const map<string, string>& stringVariables, const map<string, double>& numericVariables) {
    stack<double> values;
    stack<char> operators;

    // 解析数字、变量或字符串常量
    auto resolveVariable = [&](const string& token) -> double {
        if (isdigit(token[0]) || (token[0] == '-' && token.size() > 1)) {
            return stod(token); // 数字直接转换
        }
        if (token[0] == '\'' && token[token.size() - 1] == '\'') {
            return token.size() > 2 ? 1.0 : 0.0; // 字符串非空为1，空为0
        }
        auto itNum = numericVariables.find(token);
        if (itNum != numericVariables.end()) {
            return itNum->second; // 从数字变量表中获取值
        }
        auto itStr = stringVariables.find(token);
        if (itStr != stringVariables.end()) {
            return !itStr->second.empty() ? 1.0 : 0.0; // 字符串非空为1，空为0
        }
        throw runtime_error("Undefined variable: " + token);
    };

    // 运算符优先级
    auto precedence = [](char op) -> int {
        if (op == '&') return 1;  // && 对应 &
        if (op == '|') return 0;  // || 对应 |
        return 2;  // 数字运算符的优先级
    };

    // 执行数值运算
    auto applyOp = [](double a, double b, char op) -> double {
        if (op == '=') return a == b ? 1.0 : 0.0;  // 用==判断
        if (op == '!') return a != b ? 1.0 : 0.0; // 用!=判断
        if (op == '&') return a * b; // 逻辑与用乘法代替
        if (op == '|') return a + b; // 逻辑或用加法代替
        throw runtime_error("Unsupported operator");
    };

    // 预处理表达式：加空格
    string expr = expression;
    for (size_t i = 0; i < expr.length(); ++i) {
        if (expr[i] == '&' && i + 1 < expr.length() && expr[i + 1] == '&') {
            expr[i] = ' '; // 将&&转换为空格，后续处理为&
            expr[i + 1] = '&';
        } else if (expr[i] == '|' && i + 1 < expr.length() && expr[i + 1] == '|') {
            expr[i] = ' '; // 将||转换为空格，后续处理为|
            expr[i + 1] = '|';
        } else if (ispunct(expr[i]) && !isspace(expr[i])) {
            expr.insert(i, " "); // 其他符号添加空格
            expr.insert(i + 2, " ");
            i += 2;
        }
    }

    stringstream ss(expr);
    string token;

    // 处理表达式
    while (ss >> token) {
        if (token == "(") {
            operators.push('('); // 左括号
        } else if (token == ")") {
            // 计算括号内的表达式
            while (!operators.empty() && operators.top() != '(') {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = operators.top(); operators.pop();
                values.push(applyOp(a, b, op));
            }
            operators.pop(); // 弹出 '('
        } else if (token == "&" || token == "|") {
            // 处理逻辑运算符
            while (!operators.empty() && precedence(operators.top()) >= precedence(token[0])) {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = operators.top(); operators.pop();
                values.push(applyOp(a, b, op));
            }
            operators.push(token[0]);
        } else if (token == "=" || token == "!") {
            // 处理比较操作符
            double rhs = values.top(); values.pop();
            double lhs = resolveVariable(token); // 左侧是数值或变量
            values.push(applyOp(lhs, rhs, token[0]));
        } else {
            // 解析数字或变量
            values.push(resolveVariable(token));
        }
    }

    // 计算剩余的运算符
    while (!operators.empty()) {
        double b = values.top(); values.pop();
        double a = values.top(); values.pop();
        char op = operators.top(); operators.pop();
        values.push(applyOp(a, b, op));
    }

    return values.top() != 0; // 返回最终的布尔值（非零为true，零为false）
}

int main() {
    // 示例，字符串变量和数值变量
    map<string, string> stringVariables = {{"Name", "sysy"}};
    map<string, double> numericVariables = {{"ID", 6}, {"studentID", 3}};

    string expr = "Name = 'sysy' & (ID > 5 | studentID < 4)";
    try {
        bool result = evaluateBool(expr, stringVariables, numericVariables);
        cout << "Result: " << result << endl; // 输出结果
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    system("pause");
    return 0;
}
