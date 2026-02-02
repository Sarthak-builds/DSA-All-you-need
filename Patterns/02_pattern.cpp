#include <iostream>
using namespace std;

int main () {
      for (int i = 0; i < 4; i++) {
    for (int j = 0; j <= i; j++) { // j depends on i
        cout << "*";
    }
    cout << endl;
}
   
    return 0;
}