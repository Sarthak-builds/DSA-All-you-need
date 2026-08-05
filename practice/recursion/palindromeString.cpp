#include <iostream>
#include <string>

using namespace std;

// Only need to check the first half against its mirror on the second half;
// once i reaches n/2 every pair has already been compared, so it's a palindrome.
// Cast s.size() to int: it's size_t (unsigned), and mixing it with signed i
// in arithmetic/comparisons risks signed/unsigned bugs.
bool isPalindrome(string &s, int i) {
    int n = (int)s.size();
    if (i >= n / 2) return true;
    if (s[i] != s[n - i - 1]) return false;
    return isPalindrome(s, i + 1);
}

int main() {
    string s;

    cout << "Enter string: ";
    cin >> s;

    cout << (isPalindrome(s, 0) ? "true" : "false") << endl;

    return 0;
}

// Time Complexity: O(n) - each call compares one mirrored pair, at most n/2 calls.
// Space Complexity: O(n) - recursion depth grows to n/2 before any call returns.
