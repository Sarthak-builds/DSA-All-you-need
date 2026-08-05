#include <iostream>
#include <vector>

using namespace std;

// Find the value with the highest frequency and the value with the lowest
// frequency in an array. Same build -> precompute pattern as frequencyCount.cpp;
// the only new part is a single extra pass over the (much smaller) hash array
// to find the max/min counts, instead of a per-query fetch.
int main() {
    int n;

    cout << "Enter array size: ";
    cin >> n;

    vector<int> arr(n);
    int maxVal = 0;
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

    // step 3: scan the hash array (not the original array) to find highest/lowest frequency
    int highestFreq = -1, highestVal = -1;
    int lowestFreq = n + 1, lowestVal = -1;
    for (int val = 0; val <= maxVal; val++) {
        if (hash[val] == 0) continue;   // value never appeared, skip it
        if (hash[val] > highestFreq) {
            highestFreq = hash[val];
            highestVal = val;
        }
        if (hash[val] < lowestFreq) {
            lowestFreq = hash[val];
            lowestVal = val;
        }
    }

    cout << "Highest frequency: " << highestVal << " appears " << highestFreq << " times" << endl;
    cout << "Lowest frequency: " << lowestVal << " appears " << lowestFreq << " times" << endl;

    return 0;
}

// Time Complexity: O(n + maxVal) - O(n) to precompute the hash array, then O(maxVal)
// to scan it once for the highest/lowest counts.
// Space Complexity: O(maxVal) - the hash array has maxVal + 1 slots.
