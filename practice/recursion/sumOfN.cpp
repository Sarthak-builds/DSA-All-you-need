#include <iostream>

using namespace std;

// Functional recursion: no extra bookkeeping parameter, the answer is the return value,
// combined AFTER the recursive call returns (on the way back up the call stack).
int sumOfN(int n) {
    if (n == 0) return 0;
    return n + sumOfN(n - 1);
}

int main() {
    int n;

    cout << "Enter N: ";
    cin >> n;

    cout << sumOfN(n) << endl;

    return 0;
}

// Time Complexity: O(n) - one call per number from n down to 0, O(1) work per call.
// Space Complexity: O(n) - n decreases toward 0 before any call returns, so max call stack depth is n.
