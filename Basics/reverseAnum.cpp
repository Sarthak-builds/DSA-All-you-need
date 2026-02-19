class Solution {
public:
    int reverse(int x) {
        int ans=0;
        while ( x!=0 ) {
            int val = x % 10;
            ans = (ans*10) + val;
            x=x/10;
          
        }
        return ans;
    }
};