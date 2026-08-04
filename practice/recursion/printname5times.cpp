#include <iostream>
#include <string>

using namespace std;

// const string& avoids copying "name" on every recursive call; const stops it being modified
void printNameNTimes(const string& name, int n) {
    if (n == 0) return;
    cout << name << endl;
    printNameNTimes(name, n - 1);
}

int main() {
    string name;
    int n;

    cout << "Enter your name: ";
    cin >> name;

    cout << "Enter n: ";
    cin >> n;

    printNameNTimes(name, n);

    return 0;
}

// Time Complexity: O(n) - one recursive call per remaining print, base case stops it after n calls, O(1) work per call.
// Space Complexity: O(n) - each pending call stays on the call stack until it returns, so max stack depth equals n.
