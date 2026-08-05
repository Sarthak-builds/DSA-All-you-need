#include <iostream>
#include <vector>

using namespace std;

// The core array-hashing pattern (docs/hashing.md sec 3): build -> precompute -> fetch.
// Step 1 (build): size the hash array to maxVal + 1 so every value 0..maxVal has a slot.
// Step 2 (precompute): one O(n) pass over arr, counting each value into its own slot.
// Step 3 (fetch): every later query is a single O(1) array read - no re-scanning arr.
int main() {
    int n, maxVal;

    cout << "Enter array size: ";
    cin >> n;

    vector<int> arr(n);
    maxVal = 0;
    cout << "Enter " << n << " non-negative elements: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
        maxVal = max(maxVal, arr[i]);
    }

    // step 1: build
    vector<int> hash(maxVal + 1, 0);

    // step 2: pre-calculation
    for (int i = 0; i < n; i++)
        hash[arr[i]]++;

    // step 3: fetching
    int q;
    cout << "Enter number of queries: ";
    cin >> q;
    while (q--) {
        int val;
        cout << "Query value: ";
        cin >> val;
        if (val < 0 || val > maxVal) {
            cout << "0 (out of range, never appeared)" << endl;
        } else {
            cout << hash[val] << endl;
        }
    }

    return 0;
}

// Time Complexity: O(n + q) - O(n) for the one-time precompute pass, then O(1) per
// query (q of them) since fetching is a direct array read.
// Space Complexity: O(maxVal) - the hash array has maxVal + 1 slots, independent of
// how many queries follow.
