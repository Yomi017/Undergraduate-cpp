#include <bits/stdc++.h>
using namespace std;
int main(){
    string s;
    cin >> s;
    vector<char> conpressed;
    vector<int> conpress;
    int ip = 1;
    conpressed.push_back(s[0]);
    for(int i = 1; i < s.size(); i++){
        if(s[i] == s[i-1]){
            ++ip;
        }
        else{
            conpress.push_back(ip);
            ip = 1;
            conpressed.push_back(s[i]);
        }
    }
    conpress.push_back(ip);
    for(int i = 0; i < conpressed.size(); i++){
        cout << conpressed[i];
        if(conpress[i] != 1) cout << conpress[i];
    }
}