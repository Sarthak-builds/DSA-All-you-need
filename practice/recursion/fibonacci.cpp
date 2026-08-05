#include <iostream>

using namespace std;

// Each call branches into two smaller calls until it hits a base case;
// no memoization, so overlapping subproblems (e.g. fib(3) inside both
// fib(5) and fib(4)) get recomputed from scratch every time.
int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main() {
    int n;

    cout << "Enter N: ";
    cin >> n;

    cout << fibonacci(n) << endl;

    return 0;
}

// Time Complexity: O(2^n) - each call spawns two more calls until the base case,
// forming a binary tree of roughly 2^n nodes with no work reused between branches.
// Space Complexity: O(n) - only one root-to-leaf path is on the call stack at a time,
// and the deepest path has n calls.
