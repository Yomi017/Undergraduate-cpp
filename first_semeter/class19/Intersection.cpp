#include <bits/stdc++.h>
using namespace std;
vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
    vector<int> res;
    unordered_map<int, int> mp;
    for (int i = 0; i < nums1.size(); i++) {
        mp[nums1[i]]++;
    }
    for (int i = 0; i < nums2.size(); i++) {
        if (mp[nums2[i]] > 0) {
            res.push_back(nums2[i]);
            mp[nums2[i]]--;
        }
    }
    return res;
}
int main(){
    vector<int> nums1;
    vector<int> nums2;
    int n, m;
    cin >> n;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        nums1.push_back(x);
    }
    cin >> m;
    for (int i = 0; i < m; i++) {
        int x;
        cin >> x;
        nums2.push_back(x);
    }
    vector<int> res = intersection(nums1, nums2);
    sort(res.begin(), res.end());
    cout << "[";
    for (int i = 0; i < res.size(); i++) {
        if (i > 0) cout << ", ";
        cout << res[i];
    }
    cout << "]" << endl;
    return 0;
}