#include<bits/stdc++.h>
using namespace std;
class C{
    public:
    C(){
        C::logs += "1";
    }
    C(C &c){
       C::logs += "2";
    }
    ~C(){
      C::logs += "3";
    }
    static string logs;
};
void fun(C c){
    
}
void boo(C &c){
    
}
void goo(C *c){
    
}
string C::logs = "";
int main(){
    C arr[3];
    C *ptr = new C[2];
    for (int i = 0; i < 2; i++){
        fun(arr[i]);
        boo(arr[i]);
        goo(arr + i);
    }
    delete []ptr;
    cout<<C::logs<<endl;
    system("pause");
    return 0;
}