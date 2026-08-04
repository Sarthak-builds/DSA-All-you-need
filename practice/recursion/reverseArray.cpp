#include <iostream>

using namespace std;

// Parameterized recursion: start/end are threaded through like two pointers closing in
// from opposite ends. Each call swaps its pair in place, then hands off the next pair inward.
void reverseArray(int arr[], int start, int end) {
    if (start >= end) return;
    swap(arr[start], arr[end]);
    reverseArray(arr, start + 1, end - 1);
}

int main() {
    int n;

    cout << "Enter array size: ";
    cin >> n;

    int arr[n];
    cout << "Enter " << n << " elements: ";
    for (int i = 0; i < n; i++) cin >> arr[i];

    reverseArray(arr, 0, n - 1);

    for (int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << endl;

    return 0;
}

// Time Complexity: O(n) - n/2 swaps, each call does O(1) work.
// Space Complexity: O(n) - start/end close in toward each other before any call returns, so max call stack depth is n/2, which is O(n).
