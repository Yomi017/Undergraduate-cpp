#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

void totalBS(vector<int> &arr){
    int total = 0;
    int n = arr.size();
    for (int i = 0; i < pow(2, n); i++){
        int plus = 0;
        int times = 1;        
        for (int j = 0; j < n; j++){
            if (i & (1 << j)) {
                plus += arr[j];
                times *= arr[j];
            }
        }
        if (plus == times){
            total++;
        }
    }

    cout << total << endl;
}

int main(){
    int n;
    cin >> n;
    vector<int> arr(n);
    for(int i = 0; i < n; i++){
        cin >> arr[i];
    }
    totalBS(arr);
    return 0;
}
