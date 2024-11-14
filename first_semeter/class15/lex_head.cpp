#include <bits/stdc++.h>
#include "../project/project.h"
using namespace std;

template <typename V>
class Lexer {
public:
    Lexer(function<optional<pair<V, string>>(const string&)> parseFunc) : _(parseFunc) {}

    optional<pair<V, string>> parse(const string& str) const {
        return _(str);
    }

private:
    function<optional<pair<V, string>>(const string&)> _;
};

// map: 映射函数
template <typename I, typename O>
function<optional<pair<O, string>>(const string&)> 
map(const Lexer<I>& self, const function<optional<O>(I)>& f) {
    return [=](const string& input) -> optional<pair<O, string>> {
        auto result = self.parse(input);
        if (result) {
            auto& [token, rest] = *result;
            auto transformed = f(token);
            if (transformed) {
                return make_optional(make_pair(*transformed, rest));
            }
        }
        return nullopt;
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
function<optional<pair<Value, string>>(const string&)>
or_lexer(const Lexer<Value>& self, const Lexer<Value>& parser2) {
    return [=](const string& input) -> optional<pair<Value, string>> {
        auto result = self.parse(input);
        if (result) {
            return result;
        } else {
            return parser2.parse(input);
        }
    };
}

// many: 重复解析器
template <typename Value>
function<optional<pair<vector<Value>, string>>(const string&)>
many(const Lexer<Value>& self) {
    return [=](const string& input) -> optional<pair<vector<Value>, string>> {
        vector<Value> result;
        string rest = input;
        while (true) {
            auto r = self.parse(rest);
            if (r) {
                auto& [value, new_rest] = *r;
                result.push_back(value);
                rest = new_rest;
            } else {
                break;
            }
        }
        return make_optional(make_pair(result, rest));
    };
}

// filter: 过滤解析器
template <typename Value>
function<optional<pair<Value, string>>(const string&)>
filter(const Lexer<Value>& self, const function<bool(Value)>& f) {
    return [=](const string& input) -> optional<pair<Value, string>> {
        auto result = self.parse(input);
        if (result) {
            auto& [value, rest] = *result;
            if (f(value)) {
                return result; // 返回成功的解析结果
            }
        }
        return nullopt; // 解析失败或过滤条件不满足
    };
}


