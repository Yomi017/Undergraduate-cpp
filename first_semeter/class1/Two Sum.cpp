#include <bits/stdc++.h>
using namespace std;
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int q) {
      int a = nums.size();
      for(int i=0;i< a; i++){
        for(int j= 0;j< i; j++){
            if(nums[i]+nums[j]== q){
                return vector <int>{j,i};
            }
        }
      }
    }
};