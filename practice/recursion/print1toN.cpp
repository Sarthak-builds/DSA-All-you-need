#include <iostream>

using namespace std;

void print1toN(int i, int n) {
    if (i > n) return;
    cout << i << " ";
    print1toN(i + 1, n);
}

int main() {
    int n;

    cout << "Enter N: ";
    cin >> n;

    print1toN(1, n);
    cout << endl;

    return 0;
}

// Time Complexity: O(n) - one call per number from 1 to n, O(1) work per call.
// Space Complexity: O(n) - i increases toward n before any call returns, so max call stack depth is n.
