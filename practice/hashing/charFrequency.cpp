#include <iostream>
#include <string>

using namespace std;

// Character hashing (docs/hashing.md sec 6): same build -> precompute -> fetch
// pattern as frequencyCount.cpp, but the hash array is fixed at size 26 and
// `c - 'a'` shifts each lowercase letter into a valid 0..25 index.
int main() {
    string s;
    cin >> s;

    // step 1 + 2: build and pre-compute
    int hash[26] = {0};
    for (int i = 0; i < (int)s.size(); i++) {
        hash[s[i] - 'a']++;
    }

    // step 3: fetching
    int q;
    cin >> q;
    while (q--) {
        char c;
        cin >> c;
        cout << hash[c - 'a'] << endl;
    }

    return 0;
}

// Time Complexity: O(n + q) - O(n) to precompute over the string, then O(1) per
// query (q of them) since fetching is a direct array read.
// Space Complexity: O(1) - the hash array is always exactly 26 slots, regardless
// of string length.
