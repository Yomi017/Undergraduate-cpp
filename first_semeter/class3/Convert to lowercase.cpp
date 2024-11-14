#include <bits/stdc++.h>
using namespace std;
int main(){
  string inside_h;
  cin >> inside_h;
  for (int i=0;i<inside_h.length();++i){
    if(isupper(inside_h[i])){
        inside_h[i]=tolower(inside_h[i]);
    }
  }
  cout << inside_h;
}