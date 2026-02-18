class Solution {
public:
    int hammingWeight(int n) {
        int count = 0;
        while ( n!=0) {
            if ( n&1 ) { //will return 1
                count++;
            }
            n = n >> 1; //right shift the operator 
        }
        return count;
    }
};