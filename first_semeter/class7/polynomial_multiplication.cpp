#include <bits/stdc++.h>
using namespace std;
void p_multiplication(vector<int> &p1,vector<int> &p2,int np1,int np2){
    int degree,s=0;
    degree = np1+np2;
    vector<int> outp;
    for(int i=np1+1;i<degree+1;++i){
        p1.push_back(0);
    }
    for(int i=np2+1;i<degree+1;++i){
        p2.push_back(0);
    }
    for(int i=0;i<degree+1;++i){
        for(int j=0;j<i+1;++j){
            s=s+p1[j]*p2[i-j];
        }
        outp.push_back(s);
        s=0;
    }
    reverse(outp.begin(),outp.end());
        if(outp[0]==1){
            cout << "x^"<< degree;
        }
        if(outp[0]==-1){
            cout << "-x^"<<degree;
        }
        if(outp[0]!=1 && outp[0]!=-1){
        cout << outp[0] << "x^"<< degree;
        }
    for(int k=1;k<degree-1;++k){
        if(outp[k]==1){
            cout << "+" <<"x^" << degree-k;
        }
        if(outp[k]==-1){
            cout << "-" <<"x^" << degree-k;
        }
        if(outp[k]<0 && outp[k] != -1){
            cout << outp[k] <<"x^" << degree-k;
        }
        if(outp[k]>0 && outp[k] != 1){
            cout << "+" << outp[k] <<"x^" << degree-k;
        }
        if(outp[k]==0){
            continue;
        }
    }
    if(outp[degree-1]==1){
        if(outp[degree]==0){
            cout << "+" << "x";
        }
        if(outp[degree]>0){
            cout << "+" <<"x"<<"+"<< outp[degree];
        }
        if(outp[degree]<0){
            cout << "+" <<"x"<< outp[degree];
        }
    }
    if(outp[degree-1]==-1){
        if(outp[degree]==0){
            cout << "-x";
        }
        if(outp[degree]>0){
            cout <<"-x"<<"+"<< outp[degree];
        }
        if(outp[degree]<0){
            cout <<"-x"<< outp[degree];
        }
    }
    if(outp[degree-1]==0){
        if(outp[degree]==0){
            cout << "";
        }
        if(outp[degree]>0){
            cout <<"+"<< outp[degree];
        }
        if(outp[degree]<0){
            cout << outp[degree];
        }
    }
    if(outp[degree-1]>0 && outp[degree-1]!=1){
        if(outp[degree]==0){
            cout << "+" << outp[degree-1] << "x";
        }
        if(outp[degree]>0){
            cout << "+" << outp[degree-1] <<"x"<<"+"<< outp[degree];
        }
        if(outp[degree]<0){
            cout << "+" << outp[degree-1] <<"x"<< outp[degree];
        }
    }
    if(outp[degree-1]<0 && outp[degree-1]!=-1){
        if(outp[degree]==0){
            cout << outp[degree-1] << "x";
        }
        if(outp[degree]>0){
            cout << outp[degree-1] <<"x"<<"+"<< outp[degree];
        }
        if(outp[degree]<0){
            cout << outp[degree-1] <<"x"<< outp[degree];
        }
    }
}
int main(){
    vector<int> p1;
    vector<int> p2;
    int input,np1,np2;
    cin >> np1;
    for(int i=0;i<np1+1;++i){
        cin >> input;
        p1.push_back(input);
    }
    reverse(p1.begin(),p1.end());
    cin >> np2;
    for(int i=0;i<np2+1;++i){
        cin >> input;
        p2.push_back(input);
    }
    reverse(p2.begin(),p2.end());
    p_multiplication(p1,p2,np1,np2);
}