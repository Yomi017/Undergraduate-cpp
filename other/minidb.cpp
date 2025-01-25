#include <bits/stdc++.h>
using namespace std;

// Utility function to trim whitespace from a string
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

// Function to split a string by a delimiter
vector<string> split(const string& str, char delim) {
    vector<string> tokens;
    istringstream iss(str);
    string token;
    while (getline(iss, token, delim)) {
        tokens.push_back(trim(token));
    }
    return tokens;
}

class Database {
public:
    void createTable(const string& tableName, const vector<pair<string, string>>& columns) {
        tables[tableName] = {columns, {}};
    }

    map<string, pair<vector<pair<string, string>>, vector<map<string, string>>>> getTables() const {
        return tables;
    }

    map<string, pair<vector<pair<string, string>>, vector<map<string, string>>>> tables;
};

class MiniDB {
public:
    void createDatabase(const string& dbName) {
        databases[dbName] = Database();
    }

    void useDatabase(const string& dbName) {
        currentDatabase = dbName;
    }

    void createTable(const string& tableName, const vector<pair<string, string>>& columns) {
        if (!currentDatabase.empty() && databases.find(currentDatabase) != databases.end()) {
            databases[currentDatabase].createTable(tableName, columns);
        } else {
            cerr << "No database selected or database does not exist." << endl;
        }
    }

    void dropTable(const string& tableName) {
        if (!currentDatabase.empty() && databases.find(currentDatabase) != databases.end()) {
            databases[currentDatabase].tables.erase(tableName);
        } else {
            cerr << "No database selected or database does not exist." << endl;
        }
    }

    void insert_data(const string& tableName, const vector<string>& values) {
        if (!currentDatabase.empty() && databases.find(currentDatabase) != databases.end()) {
            auto& db = databases[currentDatabase];
            if (db.tables.find(tableName) != db.tables.end()) {
                auto& table = db.tables[tableName];
                if (values.size() != table.first.size()) {
                    cerr << "Column count doesn't match." << endl;
                    return;
                }

                map<string, string> row;
                for (size_t i = 0; i < values.size(); ++i) {
                    row[table.first[i].first] = values[i];
                }
                table.second.push_back(row);
            } else {
                cerr << "Table does not exist." << endl;
            }
        } else {
            cerr << "No database selected or database does not exist." << endl;
        }
    }

    void select_data(const vector<string>& columns, const string& tableName, const string& wherePart) {
        if (!currentDatabase.empty() && databases.find(currentDatabase) != databases.end()) {
            auto& db = databases[currentDatabase];
            if (db.tables.find(tableName) != db.tables.end()) {
                auto& table = db.tables[tableName];
                vector<map<string, string>> data;

                // Parse WHERE conditions
                vector<tuple<string, string, string>> conditions; // Tuple of column name, operator, and value
                vector<string> logicalOperators; // AND, OR
                
                if (!wherePart.empty()) {
                    istringstream iss(wherePart);
                    string condition;
                    while (getline(iss, condition, ' ')) {
                        if (condition == "AND" || condition == "OR") {
                            logicalOperators.push_back(condition);
                        } else {
                            size_t opPos = condition.find_first_of("=<>!");
                            if (opPos != string::npos) {
                                string colName = trim(condition.substr(0, opPos));
                                string op = trim(condition.substr(opPos, condition.find_first_not_of("=<>!", opPos) - opPos));
                                string value = trim(condition.substr(opPos + op.size()));
                                conditions.emplace_back(colName, op, value);
                            }
                        }
                    }
                }

                // Evaluate WHERE conditions
                for (const auto& row : table.second) {
                    bool match = true;
                    for (size_t i = 0; i < conditions.size(); ++i) {
                        const string& colName = get<0>(conditions[i]);
                        const string& op = get<1>(conditions[i]);
                        const string& value = get<2>(conditions[i]);

                        if (row.find(colName) == row.end()) {
                            match = false;
                            break; // Column does not exist
                        }

                        const string& rowValue = row.at(colName);

                        if (!evaluateCondition(rowValue, op, value)) {
                            match = false;
                            if (i < logicalOperators.size() && logicalOperators[i] == "OR") {
                                match = true; // OR condition allows match
                            } else {
                                break; // AND condition fails
                            }
                        }
                    }
                    if (match) {
                        data.push_back(row);
                    }
                }

                // Output selected columns
                for (const auto& row : data) {
                    for (size_t i = 0; i < columns.size(); ++i) {
                        const auto& colName = columns[i];
                        if (row.find(colName) != row.end()) {
                            if (i > 0) cout << ",";
                            cout << row.at(colName);
                        } else {
                            cerr << "Column " << colName << " does not exist in table " << tableName << "." << endl;
                        }
                    }
                    cout << endl;
                }
            } else {
                cerr << "Table does not exist." << endl;
            }
        } else {
            cerr << "No database selected or database does not exist." << endl;
        }
    }

    void delete_data(const string& tableName, const string& wherePart) {
        if (!currentDatabase.empty() && databases.find(currentDatabase) != databases.end()) {
            auto& db = databases[currentDatabase];
            if (db.tables.find(tableName) != db.tables.end()) {
                auto& table = db.tables[tableName];
                // WHERE clause logic should be implemented here.
                table.second.clear(); // Clear all data for simplicity
            } else {
                cerr << "Table does not exist." << endl;
            }
        } else {
            cerr << "No database selected or database does not exist." << endl;
        }
    }

    void update_data(const string& setPart, const string& wherePart) {
        if (!currentDatabase.empty() && databases.find(currentDatabase) != databases.end()) {
            // Implement update logic here
        } else {
            cerr << "No database selected or database does not exist." << endl;
        }
    }

    void outputCSV(const string& outputFile) {
        ofstream ofs(outputFile);

        if (!currentDatabase.empty() && databases.find(currentDatabase) != databases.end()) {
            for (const auto& table : databases[currentDatabase].getTables()) {
                const auto& columns = table.second.first;
                const auto& data = table.second.second;

                // Write header
                for (size_t i = 0; i < columns.size(); ++i) {
                    ofs << columns[i].first;
                    if (i < columns.size() - 1) ofs << ",";
                }
                ofs << endl;

                // Write data
                for (const auto& row : data) {
                    for (size_t i = 0; i < columns.size(); ++i) {
                        const auto& colName = columns[i].first;
                        const auto& colType = columns[i].second;
                        const auto& value = row.at(colName);

                        if (colType == "TEXT") {
                            ofs << "\"" << value << "\"";
                        } else if (colType == "FLOAT") {
                            ofs << fixed << setprecision(2) << stof(value);
                        } else {
                            ofs << value; // INTEGER or other types
                        }

                        if (i < columns.size() - 1) ofs << ",";
                    }
                    ofs << endl;
                }
            }
        } else {
            cerr << "No database selected or database does not exist." << endl;
        }

        ofs.close();
    }

    bool evaluateCondition(const string& rowValue, const string& op, const string& value) {
        // Convert rowValue and value to float if they are numbers
        float rowValNum, valNum;
        istringstream rowStream(rowValue);
        istringstream valStream(value);

        bool isRowValNum = (rowStream >> rowValNum) && (rowStream.eof());
        bool isValNum = (valStream >> valNum) && (valStream.eof());
        if (isRowValNum && isValNum) {
            if (op == "=") return rowValNum == valNum;
            if (op == "<") return rowValNum < valNum;
            if (op == ">") return rowValNum > valNum;
            if (op == "<=") return rowValNum <= valNum;
            if (op == ">=") return rowValNum >= valNum;
            if (op == "!=") return rowValNum != valNum;
        } else {
            // If not numbers, do string comparison
            if (op == "=") return rowValue == value;
            if (op == "!=") return rowValue != value;
            cerr << "Invalid operator or non-numeric values for comparison." << endl;
        }
        return false; // Return false if either conversion fails or invalid comparison
    }

    map<string, Database> databases;
    string currentDatabase;
};

void parseCommand(const string& command, MiniDB& minidb) {
    istringstream iss(command);
    string action;
    iss >> action;

    if (action == "CREATE") {
        string objType;
        iss >> objType;

        if (objType == "DATABASE") {
            string dbName;
            iss >> dbName;
            minidb.createDatabase(dbName);
        } else if (objType == "TABLE") {
            string tableName;
            iss >> tableName;
            vector<pair<string, string>> columns;
            string columnDef;

            // Read column definitions
            getline(iss, columnDef, '('); // Skip to the '(' character
            getline(iss, columnDef, ')'); // Read until the ')' character
            istringstream colStream(columnDef);
            string col;
            while (getline(colStream, col, ',')) {
                string colName, colType;
                istringstream colEntry(col);
                colEntry >> colName >> colType;
                columns.push_back({colName, colType});
            }

            minidb.createTable(tableName, columns);
        }
    } else if (action == "USE") {
        string ignore, dbName;
        iss >> ignore >> dbName; // Skip the "DATABASE" keyword
        minidb.useDatabase(dbName);
    } else if (action == "DROP") {
        string objType;
        iss >> objType;
        if (objType == "TABLE") {
            string tableName;
            iss >> tableName;
            minidb.dropTable(tableName);
        }
    } else if (action == "INSERT") {
        string ignore;
        iss >> ignore; // skip INTO
        string tableName;
        iss >> tableName;
        vector<string> values;
        string value;
        getline(iss >> ws, value);
        value = value.substr(value.find("(") + 1); // Find values between '(' and ')'
        value = value.substr(0, value.find(")"));
        istringstream valStream(value);
        string val;
        while (getline(valStream, val, ',')) {
            val.erase(remove(val.begin(), val.end(), '\''), val.end()); // Remove quotes
            values.push_back(val);
        }
        minidb.insert_data(tableName, values);
    } else if (action == "SELECT") {
        vector<string> columns;
        string columnDef;
        
        getline(iss, columnDef, 'F'); // read until 'FROM'
        istringstream colStream(columnDef);
        string col;
        while (getline(colStream, col, ',')) {
            columns.push_back(trim(col));
        }

        string fromWord;
        iss >> fromWord;
        string tableName;
        iss >> tableName;

        // Optional WHERE condition
        string wherePart;
        if (iss >> ws && iss.peek() == 'W') {
            getline(iss, wherePart);
            // Remove leading "WHERE "
            if (wherePart.substr(0, 5) == "WHERE") {
                wherePart = wherePart.substr(5);
            }
        }

        minidb.select_data(columns, tableName, wherePart);
    } else if (action == "DELETE") {
        string ignore;
        iss >> ignore; // skip FROM
        string tableName;
        iss >> tableName;
        string wherePart;
        if (iss >> ws && iss.peek() == 'W') {
            getline(iss, wherePart);
        }
        minidb.delete_data(tableName, wherePart);
    } else if (action == "UPDATE") {
        string tableName;
        iss >> tableName; // table name
        string setPart;
        iss >> setPart;
        string wherePart;
        if (iss >> ws && iss.peek() == 'W') {
            getline(iss, wherePart);
        }
        minidb.update_data(setPart, wherePart);
    } else {
        cerr << "Unknown command: " << action << endl;
    }
}

int main() {
    MiniDB minidb;
    ifstream inputFile("input.sql");
    string line;

    while (getline(inputFile, line)) {
        // Trim any leading or trailing whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        // Skip empty lines
        if (line.empty()) {
            continue;
        }

        parseCommand(line, minidb);
    }

    // Output the final data to the CSV file
    minidb.outputCSV("output.csv");
    return 0;
}