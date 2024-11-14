#include <bits/stdc++.h>
#include "../project/project.h"
using namespace std;

template <typename I, typename O>
std::function<std::optional<std::pair<O, std::string>>(const std::string&)>
map(const Lexer<I>& self, const std::function<std::optional<O>(I)>& f) {
    return [self, f](const std::string& input) -> std::optional<std::pair<O, std::string>> {
        auto result = self.parse(input);
        if (result) {
            auto& [token, rest] = *result;
            auto transformed = f(token);
            if (transformed) {
                return std::make_optional(std::make_pair(*transformed, rest));
            }
        }
        return std::nullopt;
    };
}

// and: 串联两个解析器
template <typename V1, typename V2>
function<optional<pair<pair<V1, V2>, string>>(const string&)>
and_lexer(const Lexer<V1>& self, const Lexer<V2>& parser2) {
    return [=](const string& input) -> optional<pair<pair<V1, V2>, string>> {
        auto result1 = self.parse(input);
        if (result1) {
            auto& [value1, rest1] = *result1;
            auto result2 = parser2.parse(rest1);
            if (result2) {
                auto& [value2, rest2] = *result2;
                return make_optional(make_pair(make_pair(value1, value2), rest2));
            }
        }
        return nullopt;
    };
}

// or: 选择两个解析器
template <typename Value>
std::function<std::optional<std::pair<Value, std::string>>(const std::string&)>
or_lexer(const Lexer<Value>& self, const Lexer<Value>& parser2) {
    return [self, parser2](const std::string& input) -> std::optional<std::pair<Value, std::string>> {
        auto result = self.parse(input);
        if (result) {
            return result;
        }
        return parser2.parse(input);
    };
}

// many: 重复解析器
template <typename Value>
std::function<std::optional<std::pair<std::vector<Value>, std::string>>(const std::string&)>
many(const Lexer<Value>& self) {
    return [=](const std::string& input) -> std::optional<std::pair<std::vector<Value>, std::string>> {
        std::vector<Value> result;
        std::string rest = input;

        // 解析一个或多个元素，直到没有更多匹配
        while (true) {
            auto r = self.parse(rest);
            if (r) {
                auto& [value, newRest] = *r;
                result.push_back(value);
                rest = newRest;  // 更新剩余部分
            } else {
                break;  // 如果没有匹配，退出循环
            }
        }

        // 返回结果
        return std::make_optional(std::make_pair(result, rest));
    };
}



// pstring: 解析字符串
template <typename T>
Lexer<T> pstring(const T& str) {
    return Lexer<T>([=](const std::string& input) -> std::optional<std::pair<T, std::string>> {
        if (input.size() >= str.size() && input.substr(0, str.size()) == str) {
            return std::make_optional(std::make_pair(str, input.substr(str.size())));
        }
        return std::nullopt;
    });
}

// pchar: 解析单个字符
// Lexer<char> pchar(char c) {
//     return Lexer<char>([=](const string& input) -> optional<pair<char, string>> {
//         if (input.size() > 0 && input[0] == c) {
//             return make_optional(make_pair(c, input.substr(1)));
//         }
//         return nullopt;
//     });
// }
