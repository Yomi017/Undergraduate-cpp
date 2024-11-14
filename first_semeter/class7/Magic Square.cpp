#include <bits/stdc++.h>
using namespace std;
int isMagicSquare(vector<vector<int>> &magic1, int N) {
    int Const = N * (N * N + 1) / 2;
    for (int i = 0; i < N; ++i) {
        int rowSum = 0;
        for (int j = 0; j < N; ++j) {
            rowSum += magic1[i][j];
        }
        if (rowSum != Const) return false;
    }
    for (int j = 0; j < N; ++j) {
        int colSum = 0;
        for (int i = 0; i < N; ++i) {
            colSum += magic1[i][j];
        }
        if (colSum != Const) return false;
    }
    int diag1 = 0, diag2 = 0;
    for (int i = 0; i < N; ++i) {
        diag1 += magic1[i][i];
        diag2 += magic1[i][N - i - 1];
    }
    if (diag1 != Const || diag2 != Const) return false;

    return true;
}
void isMS(bool isM,int N,vector<vector<int>> &magic1,int &b){
    if(isM==true){
        for(int i=0; i<N;++i){
            for(int j=0; j<N;++j){
                cout << magic1[i][j]<<" ";
            }
            cout << endl;
            b=1;
        }
        return;
    }
}
void Magic(vector<vector<int>> &magic1,int N,vector<int> &posx,vector<int> &posy){
    vector<int> n;
    vector<int> n1;
    for(int i=0; i<N;++i){
        for(int j=0; j<N;++j){
            n1.push_back(magic1[i][j]);
        }
    }
    for(int i=1; i<N*N+1;++i){
        n.push_back(i);
    }
    for(int i=0;i<N*N;++i){
        n.erase(remove(n.begin(), n.end(), n1[i]), n.end());
    }
    int b=0;
    do{
    magic1[posx[0]][posy[0]]=n[0];
    magic1[posx[1]][posy[1]]=n[1];
    magic1[posx[2]][posy[2]]=n[2];
    isMS(isMagicSquare(magic1,N),N,magic1,b);
    if(b==1){
        return;
    }
    }
    while(next_permutation(n.begin(),n.end()));
    if(b==0){
        cout << "No solution";
        return;
    }
}
int main(){
    int N;
    cin >> N;
    vector<vector<string>> magic(N, vector<string>(N));
    vector<vector<int>> magic1(N, vector<int>(N));
    vector<int> posx;
    vector<int> posy;
    for(int i=0; i<N;++i){
        for(int j=0; j<N;++j){
            cin >> magic[i][j];
            if(magic[i][j]=="_"){
                magic[i][j]="-1";
                posx.push_back(i);
                posy.push_back(j);
            }
        }
    }
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
            magic1[i][j] = std::stoi(magic[i][j]);
            }
    }
    Magic(magic1,N,posx,posy);
    }