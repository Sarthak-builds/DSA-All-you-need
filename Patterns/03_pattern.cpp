
#include <iostream>
using namespace std;

int main () {
   for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4 - i; j++) { // Shrinks as i grows
        cout << "*";
    }
    cout << endl;
}
    return 0;
}