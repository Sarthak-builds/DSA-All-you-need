#include <iostream>

using namespace std;

// Parameterized recursion: "acc" is threaded through as a parameter and updated on the way
// DOWN, like a loop variable. By the time the base case hits, acc already holds the answer -
// there's nothing left to combine on the way back up.
void sumOfN(int i, int n, int acc) {
    if (i > n) {
        cout << acc << endl;
        return;
    }
    sumOfN(i + 1, n, acc + i);
}

int main() {
    int n;

    cout << "Enter N: ";
    cin >> n;

    sumOfN(1, n, 0);

    return 0;
}

// Time Complexity: O(n) - one call per number from 1 to n, O(1) work per call.
// Space Complexity: O(n) - i increases toward n before any call returns, so max call stack depth is n.
