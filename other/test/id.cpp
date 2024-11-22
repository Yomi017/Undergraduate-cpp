#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <stack>
#include <stdexcept>

using namespace std;

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


int main() {
    // 定义变量 x = 5
    map<string, double> variables;
    variables["x"] = 5;

    // 计算表达式 "2+(x-3)*2"
    string expression = "x + 1"; // 请注意，运算符两边有空格，以便正确分隔
    double result = evaluate(expression, variables);

    // 输出结果
    cout << "Result: " << result << endl;

    return 0;
}
