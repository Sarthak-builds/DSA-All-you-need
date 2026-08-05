#include <iostream>
#include <vector>

using namespace std;

// Problem: Print just ONE subsequence of an array that sums to k (any one is fine,
// stop as soon as one is found - don't explore further branches after that).
//
// Same pick/not-pick shape as subsequenceSumK.cpp, but now the function needs to
// RETURN bool (functional recursion, see docs/recursion.md sec 8.1): true means
// "a valid subsequence was found somewhere below this call, already printed" -
// once a branch returns true, the caller must stop exploring its other branch
// and just propagate true straight back up, instead of trying the not-pick case too.
bool printOneSubsequenceWithSumK(vector<int>& arr, int index, vector<int>& subArr, int sum, int k) {
    // base case
    if (index >= (int)arr.size()) {
        if (sum == k) {
            for (int val : subArr) cout << val << " ";
            cout << endl;
            return true;
        }
        return false;
    }
    // pick
    subArr.push_back(arr[index]);
    if (printOneSubsequenceWithSumK(arr, index + 1, subArr, sum + arr[index], k)) return true;
    // not pick
    subArr.pop_back();
    return printOneSubsequenceWithSumK(arr, index + 1, subArr, sum, k);
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

    vector<int> subArr;
    bool found = printOneSubsequenceWithSumK(arr, 0, subArr, 0, k);
    if (!found) cout << "No subsequence sums to " << k << endl;

    return 0;
}

// Time Complexity: O(2^n) worst case - if no subsequence sums to k, every branch of the
// pick/not-pick tree is explored before returning false; best case can return as early as O(n)
// if the first "pick" chain happens to hit k.
// Space Complexity: O(n) - max recursion depth is n; `subArr` also grows to at most n elements.
