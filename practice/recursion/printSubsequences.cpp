#include <iostream>
#include <vector>

using namespace std;

// Problem: Print all subsequences of an array (any subset of elements,
// keeping relative order; the empty subsequence counts too).
// e.g. [1, 2, 3] -> [], [1], [2], [1,2], [3], [1,3], [2,3], [1,2,3]
//
// Pick/not-pick pattern: at each index, branch into two recursive calls -
// one that includes arr[index] in the running subsequence, one that skips it.
// Base case: index == n -> the current subsequence is complete, print it.
void printSubsequences(vector<int>& arr, int index, vector<int>& subArr) {
    // base case
    if (index >= arr.size()) {
        for (int val : subArr) cout << val << " ";
        cout << endl;
        return;
    }
    // pick
    subArr.push_back(arr[index]);
    printSubsequences(arr, index + 1, subArr);
    // not pick
    subArr.pop_back();
    printSubsequences(arr, index + 1, subArr);
}

int main() {
    int n;
    cout << "Enter array size: ";
    cin >> n;
    vector<int> arr(n);
    cout << "Enter " << n << " elements: ";
    for (int i = 0; i < n; i++) cin >> arr[i];

    vector<int> subArr;
    printSubsequences(arr, 0, subArr);

}

// Time Complexity: O(2^n) - each index branches into two calls (pick/not-pick),
// forming a binary tree of depth n with 2^n leaves, each printing O(n) elements.
// Space Complexity: O(n) - max recursion depth is n (one frame per index); `subArr`
// also grows to at most n elements, still O(n).
