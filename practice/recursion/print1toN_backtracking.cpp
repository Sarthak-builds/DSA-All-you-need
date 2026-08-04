#include <iostream>

using namespace std;

// i counts DOWN to the base case first; printing happens after the recursive call,
// so it fires while the stack unwinds (backtracks) - the smallest i prints first,
// giving 1..N even though the parameter itself was decreasing on the way down.
void print1toN(int i, int n) {
    if (i < 1) return;
    print1toN(i - 1, n);
    cout << i << " ";
}

int main() {
    int n;

    cout << "Enter N: ";
    cin >> n;

    print1toN(n, n);
    cout << endl;

    return 0;
}

// Time Complexity: O(n) - one call per number from n down to 1, O(1) work per call.
// Space Complexity: O(n) - i decreases toward 1 before any call returns, so max call stack depth is n.
