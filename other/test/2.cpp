void update_helper(Table& table, const string& column_name, const string& expression, const string& condition_column, const string& op, const string& value, int digit_or_identifier, vector<string>& variable_name, vector<TokenWithValue>::const_iterator& it, vector<TokenWithValue>::const_iterator end) {
    vector<bool> match(table.data.size(), false);
    // out <<digit_or_identifier;
    if (digit_or_identifier == 1) {
        auto its = find_if(table.columns.begin(), table.columns.end(), [&](const Column& col) {
            return col.name == condition_column;
        });
        if (its == table.columns.end()) {
            throw runtime_error("Condition column not found.");
        }
        size_t condition_index = distance(table.columns.begin(), its);

        for (size_t i = 0; i < table.data.size(); ++i) {
            const auto& target_row = table.data[i];
            const auto& target_value = target_row[condition_index];

            if (op == "=" && target_value == value) {
                match[i] = true;
            } else if (op == ">" && is_number_where(value) && is_number_where(target_value)) {
                if (stod(target_value) - stod(value) > 1e-9) {
                    match[i] = true;
                }
            } else if (op == "<" && is_number_where(value) && is_number_where(target_value)) {
                if (stod(target_value) - stod(value) < 1e-9) {
                    match[i] = true;
                }
            }
        }
        ++it;
        if (it != end && it->token == Token::AND) {
            ++it;
            if (it != end && it->token == Token::IDENTIFIER) {
                string condition_column2 = it->value;
                ++it;
                if (it != end && (it->token == Token::EQUAL || it->token == Token::GT || it->token == Token::LT || it->token == Token::NOT)) {
                    string symbol2 = it->value;
                    if (it != end && it->token == Token::NOT) {
                        ++it;
                        if (it != end && it->token != Token::EQUAL) {
                            cerr << "ERROR! Expected = after NOT." << "At column " << colnum << endl;
                            return;
                        }
                    }
                    ++it;
                    if (it != end && (it->token == Token::STRING || it->token == Token::NUMBER)) {
                        string condition_value2 = it->value;
                        auto its2 = find_if(table.columns.begin(), table.columns.end(), [&](const Column& col) {
                            return col.name == condition_column2;
                        });
                        if (its2 == table.columns.end()) {
                            throw runtime_error("Condition column not found.");
                        }
                        size_t condition_index2 = distance(table.columns.begin(), its2);

                        for (size_t i = 0; i < table.data.size(); ++i) {
                            const auto& target_row = table.data[i];
                            const auto& target_value = target_row[condition_index2];

                            if (match[i]) {
                                if (symbol2 == "=" && target_value == condition_value2) {
                                    match[i] = true;
                                } else if (symbol2 == ">" && is_number_where(condition_value2) && is_number_where(target_value)) {
                                    if (stod(target_value) - stod(condition_value2) > 1e-9) {
                                        match[i] = true;
                                    }
                                } else if (symbol2 == "<" && is_number_where(condition_value2) && is_number_where(target_value)) {
                                    if (stod(target_value) - stod(condition_value2) < 1e-9) {
                                        match[i] = true;
                                    }
                                } else {
                                    match[i] = false;
                                }
                            }
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
                cerr << "ERROR! Expected column name after AND." << "At column " << colnum << endl;
                return;
            }
        } else if (it != end && it->token == Token::OR) {
            ++it;
            if (it != end && it->token == Token::IDENTIFIER) {
                string condition_column2 = it->value;
                ++it;
                if (it != end && (it->token == Token::EQUAL || it->token == Token::GT || it->token == Token::LT || it->token == Token::NOT)) {
                    string symbol2 = it->value;
                    if (it != end && it->token == Token::NOT) {
                        ++it;
                        if (it != end && it->token != Token::EQUAL) {
                            cerr << "ERROR! Expected = after NOT." << "At column " << colnum << endl;
                            return;
                        }
                    }
                    ++it;
                    if (it != end && (it->token == Token::STRING || it->token == Token::NUMBER)) {
                        string condition_value2 = it->value;
                        auto its2 = find_if(table.columns.begin(), table.columns.end(), [&](const Column& col) {
                            return col.name == condition_column2;
                        });
                        if (its2 == table.columns.end()) {
                            throw runtime_error("Condition column not found.");
                        }
                        size_t condition_index2 = distance(table.columns.begin(), its2);

                        for (size_t i = 0; i < table.data.size(); ++i) {
                            const auto& target_row = table.data[i];
                            const auto& target_value = target_row[condition_index2];

                            if (!match[i]) {
                                if (symbol2 == "=" && target_value == condition_value2) {
                                    match[i] = true;
                                } else if (symbol2 == ">" && is_number_where(condition_value2) && is_number_where(target_value)) {
                                    if (stod(target_value) - stod(condition_value2) > 1e-9) {
                                        match[i] = true;
                                    }
                                } else if (symbol2 == "<" && is_number_where(condition_value2) && is_number_where(target_value)) {
                                    if (stod(target_value) - stod(condition_value2) < 1e-9) {
                                        match[i] = true;
                                    }
                                }
                            }
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
                cerr << "ERROR! Expected column name after OR." << "At column " << colnum << endl;
                return;
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
            throw runtime_error("Condition column not found.");
        }
        size_t condition_index = distance(table.columns.begin(), its);

        for (size_t i = 0; i < table.data.size(); ++i) {
            const auto& target_row = table.data[i];
            const auto& target_value = target_row[condition_index];

            if (op == "=" && target_value == value) {
                match[i] = true;
            } else if (op == ">" && is_number_where(value) && is_number_where(target_value)) {
                if (stod(target_value) > stod(value)) {
                    match[i] = true;
                }
            } else if (op == "<" && is_number_where(value) && is_number_where(target_value)) {
                if (stod(target_value) < stod(value)) {
                    match[i] = true;
                }
            }
        }

        // 替换表达式中的列名为 'x'
        string replaced_expression = expression;
        size_t pos = 0;
        // while ((pos = replaced_expression.find(column_name, pos)) != string::npos) {
        //     replaced_expression.replace(pos, column_name.length(), "x");
        //     pos += column_name.length();  // 移动到下一个位置
        // }

        for (size_t i = 0; i < table.data.size(); ++i) {
            if (match[i]) {
                vector<vector<Column>::iterator> col_itself;
                for (int j = 0; j < variable_name.size() ; ++j) {
                    auto col_it = find_if(table.columns.begin(), table.columns.end(), [&](const Column& col) {
                        return col.name == variable_name[j];
                    });
                    if (col_it == table.columns.end()) {
                        cerr << "Error: Column not found for variable " << variable_name[j] << endl;
                        // 可以选择跳过或继续处理错误
                    } else {
                        col_itself.push_back(col_it);
                    }
                }
                int decimal_places;
                size_t index;
                map<string, double> variables;
                if (all_of(col_itself.begin(), col_itself.end(), [&](vector<Column>::iterator it) { return it != table.columns.end(); })) {
                    for (int k = 0; k < variable_name.size(); ++k) {
                        auto col_itself_k = col_itself[k];
                        if (col_itself_k == table.columns.end()) {
                            cerr << "Error: column iterator is invalid for k=" << k << endl;
                            return;
                        }
                        size_t index_in = distance(table.columns.begin(), col_itself[k]);
                        // 构造变量表
                        string value_name = table.columns[index_in].name;
                        if (index_in >= table.data[i].size()) {
                            cerr << "Error: index " << index_in << " is out of bounds for table.data[" << i << "]" << endl;
                            return;
                            // 处理越界的情况（例如跳过当前循环或给出默认值）
                        } else {
                            variables[value_name] = stod(table.data[i][index_in]);
                            // cout << variables[value_name] << endl;
                        }
                        if (k == 0) {
                            index = index_in;
                            decimal_places = getDecimalPlaces(table.data[i][index]);
                        }
                    }
                    // 计算结果并格式化
                    try {
                        ostringstream oss;
                        oss << fixed << setprecision(decimal_places) << evaluate(replaced_expression, variables);
                        table.data[i][index] = oss.str();
                    } catch (const runtime_error& e) {
                        cerr << "Error evaluating expression: " << e.what() << endl;
                    }
                } else {
                    cerr << "Error: Column not found for variable" << endl;
                    // 可以选择跳过或继续处理错误
                }
            }
        }
    } else {
        cerr << "ERROR! Expected digit or identifier after EQUAL." << "At column " << colnum << endl;
    }
}
