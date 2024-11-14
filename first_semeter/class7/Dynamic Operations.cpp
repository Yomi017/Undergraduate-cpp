#include <bits/stdc++.h>
using namespace std;
int main() {
    vector<int> all;
    int input, target,b;
    while (true) {
        cin >> input;
        if (input == -1) {
            break;
        }
        all.push_back(input);
    }
    b=all.size();
    cin >> target;
    all.erase(remove(all.begin(), all.end(), target), all.end());
    sort(all.begin(), all.end());
    if(b != all.size()){
    for (int i = 0; i < all.size(); ++i) {
        cout << all[i] << " ";
    }
    cout << endl;}
    else{
        cout << "NOT FOUND";
    }
    return 0;
}