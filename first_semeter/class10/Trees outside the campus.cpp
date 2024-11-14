#include <bits/stdc++.h>
using namespace std;
int f(int len,int m,vector<vector<int>> &regions){
        vector<vector<int>> dp;
        sort(regions.begin(),regions.end());
        for (const auto& region : regions){
            if(dp.empty() || region[0] > dp.back()[1]){
                dp.push_back(region);
        }else{
            dp.back()[1] = max(dp.back()[1],region[1]);
        }}
        int ans = 0;
        for (const auto& region : dp){
            ans += region[1] - region[0] + 1;
        }
        return len - ans + 1;
    }
int main(){
    int len,m;
    cin >> len >> m;
    vector<vector<int>> regions(m,vector<int>(2));
    for (auto& region : regions){
        for (int& x : region){
            cin >> x;
        }
    }
    cout << f(len,m,regions) << endl;
}