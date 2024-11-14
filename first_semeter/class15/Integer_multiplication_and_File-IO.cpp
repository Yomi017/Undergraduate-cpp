#include <bits/stdc++.h>
using namespace std;

string multipliesNumber(string num1, string num2) {
    if (num1 == "0" || num2 == "0") return "0"; // 如果有一个输入为0，直接返回"0"

    int len1 = num1.size();
    int len2 = num2.size();
    int len = len1 + len2;
    vector<int> result(len, 0);

    for (int i = len1 - 1; i >= 0; i--) {
        for (int j = len2 - 1; j >= 0; j--) {
            int mul = (num1[i] - '0') * (num2[j] - '0');
            int sum = mul + result[i + j + 1];
            result[i + j] += sum / 10;
            result[i + j + 1] = sum % 10;
        }
    }

    string res = "";
    for (int num : result) {
        if (!(res.empty() && num == 0)) { // 跳过前导零
            res.push_back(num + '0');
        }
    }

    return res.empty() ? "0" : res;
}

int main() {
    ifstream file("input.txt");
    ofstream output("output.txt");

    if (file.is_open()) {
        string num1, num2;
        if (getline(file, num1) && getline(file, num2)) { // 确保读取两行
            string res = multipliesNumber(num1, num2);
            output << res << endl;
        } else {
            cout << "File format is incorrect. Ensure two lines of numbers." << endl;
        }
        file.close();
        output.close();
    } else {
        cout << "Unable to open file" << endl;
    }

    return 0;
}
