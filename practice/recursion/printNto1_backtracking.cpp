#include <iostream>

using namespace std;

// i counts UP to the base case first; printing happens after the recursive call,
// so it fires while the stack unwinds (backtracks) - the largest i prints first,
// giving N..1 even though the parameter itself was increasing on the way down.
void printNto1(int i, int n) {
    if (i > n) return;
    printNto1(i + 1, n);
    cout << i << " ";
}

int main() {
    int n;

    cout << "Enter N: ";
    cin >> n;

    printNto1(1, n);
    cout << endl;

    return 0;
}

// Time Complexity: O(n) - one call per number from 1 to n, O(1) work per call.
// Space Complexity: O(n) - i increases toward n before any call returns, so max call stack depth is n.
