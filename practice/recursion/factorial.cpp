#include <iostream>

using namespace std;

// Functional recursion: each call returns its own answer, combined with the
// smaller call's result AFTER it returns (see docs/recursion.md §2 for the full trace).
long long factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int main() {
    int n;

    cout << "Enter N: ";
    cin >> n;

    cout << factorial(n) << endl;

    return 0;
}

// Time Complexity: O(n) - one call per number from n down to 1, O(1) work per call.
// Space Complexity: O(n) - n decreases toward 1 before any call returns, so max call stack depth is n.
