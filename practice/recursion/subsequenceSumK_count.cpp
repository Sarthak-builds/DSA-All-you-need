#include <iostream>
#include <vector>

using namespace std;

// Problem: Count how many subsequences of an array sum to k (don't need to
// print them, just the count).
//
// Functional recursion (see docs/recursion.md sec 8.1): each call RETURNS a
// count instead of threading state through parameters. Base case returns 1
// if the running sum hit k, else 0 - that's the "answer" for an empty
// remaining array. Each call combines its two branches (left = pick,
// right = not-pick) by adding their counts together, since every valid
// subsequence is counted in exactly one of the two branches.
int countSubsequencesWithSumK(vector<int>& arr, int index, int sum, int k) {
    if (index == (int)arr.size()) {
        return sum == k ? 1 : 0;
    }

    int left = countSubsequencesWithSumK(arr, index + 1, sum + arr[index], k);  // pick
    int right = countSubsequencesWithSumK(arr, index + 1, sum, k);              // not pick

    return left + right;
}

int main() {
    int n, k;

    cout << "Enter array size: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter " << n << " elements: ";
    for (int i = 0; i < n; i++) cin >> arr[i];

    cout << "Enter target sum k: ";
    cin >> k;

    cout << countSubsequencesWithSumK(arr, 0, 0, k) << endl;

    return 0;
}

// Time Complexity: O(2^n) - two recursive calls per index (pick/not-pick), forming
// a binary tree of depth n -> 2^n leaves, O(1) work combining left+right per call.
// Space Complexity: O(n) - no extra array to carry (unlike printSubsequences), so
// space is just the call stack: max recursion depth is n.
