#include <bits/stdc++.h>
using namespace std;
void Transposer(int N,int M,vector<vector<int>> &matrix){
    vector<vector<int>> matrix_1(M,vector<int>(N));
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix_1[i][j] = matrix[j][i];
        }
    }
for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << matrix_1[i][j] << " ";
        }
        cout << endl;
    }
}
int main(){
    int N,M;
    cin >> N >> M;
    vector<vector<int>> matrix(N,vector<int>(M));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            cin >> matrix[i][j];
        }
    }
    Transposer(N,M,matrix);
}