// LeetCode 326. Power of Three

class Solution {
public:
    // n must shrink toward 1 by exact division; any remainder at any step
    // means 3 never evenly divides out, so n can't be 3^x.
    bool isPowerOfThree(int n) {
        if (n <= 0) return false;
        if (n == 1) return true;
        if (n % 3 != 0) return false;
        return isPowerOfThree(n / 3);
    }
};

// Time Complexity: O(log3 n) - each call divides n by 3, so depth is log base 3 of n.
// Space Complexity: O(log3 n) - call stack holds one frame per division until n reaches 1 (or fails).
