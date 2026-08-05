#include <iostream>
#include <vector>

using namespace std;

// Problem: Print all subsequences of an array whose elements sum to k.
// Same pick/not-pick pattern as printSubsequences.cpp, plus a running sum
// threaded through as a parameter (parameterized recursion) so each call
// knows its own sum without recomputing it from `subArr`.
// e.g. arr = [1, 2, 1], k = 2 -> [1,1], [2]
void printSubsequencesWithSumK(vector<int>& arr, int index, vector<int>& subArr, int sum, int k) {
    // base case
    if (index >= (int)arr.size()) {
        if (sum == k) {
            for (int val : subArr) cout << val << " ";
            cout << endl;
        }
        return;
    }
    // pick
    subArr.push_back(arr[index]);
    printSubsequencesWithSumK(arr, index + 1, subArr, sum + arr[index], k);
    // not pick
    subArr.pop_back();
    printSubsequencesWithSumK(arr, index + 1, subArr, sum, k);
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
    printSubsequencesWithSumK(arr, 0, subArr, 0, k);

    return 0;
}

// Time Complexity: O(2^n) - two recursive calls per index (pick/not-pick), forming
// a binary tree of depth n -> 2^n leaves, each printing at most O(n) elements.
// Space Complexity: O(n) - max recursion depth is n; `subArr` also grows to at
// most n elements, still O(n).
