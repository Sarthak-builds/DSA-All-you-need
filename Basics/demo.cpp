#include <iostream>
using namespace std;

int main() {
    cout<< "Hii people, AI is going to replace you, good luck"<<endl;
    int a = 2/5; //assignemnet operator
    cout << a << endl; //int/int=int
    float b = 2.0/5;
    cout << b << endl; //float/int=float
    float c= 2.4/5.0;
    cout << c << endl; //float
    
    //relational operators
    int d = 4;
    int e= 5;
    bool first = ( d==e);
    cout << first << endl; //0 which is false.

     if (d<e && d%2==0) {
         cout << "its true" << endl;
     } 
     else {
         cout << " not true" << endl;
     }
    return 0;
}