#include <iostream>

using namespace std;

void printNto1(int i) {
    if (i < 1) return;
    cout << i << " ";
    printNto1(i - 1);
}

int main() {
    int n;

    cout << "Enter N: ";
    cin >> n;

    printNto1(n);
    cout << endl;

    return 0;
}

// Time Complexity: O(n) - one call per number from n down to 1, O(1) work per call.
// Space Complexity: O(n) - i decreases toward 1 before any call returns, so max call stack depth is n.
