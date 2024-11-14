#include <bits/stdc++.h>
using namespace std;
int main(){
    int n,m;
    cin >> n >>m;
    vector<vector<double>> n_1(n, vector<double>(m, 0));
    vector<vector<double>> n_2(n, vector<double>(m, 0));
    for (auto& row:n_1 ){
        for(double& val:row){
            cin >> val;
        }
    }
    for (auto& row:n_2 ){
        for(double& val:row){
            cin >> val;
        }
    }
    cout << n_1 << endl << n_2;

}