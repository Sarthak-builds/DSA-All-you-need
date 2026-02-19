class Solution {
public:
    bool isPowerOfTwo(int n) {
        for ( int i = 0; i <31 ; i ++ ) {
            int ans = pow(2, i);
            if ( ans == n) return true;
            
        }
        return false;
    }
};
//check for value in range of 2 to the power - 2 to the power 30 as int ranges till 2 to the power 31 - 1.