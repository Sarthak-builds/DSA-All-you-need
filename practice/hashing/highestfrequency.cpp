#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

// Same idea as highestLowestFrequency.cpp, but using unordered_map instead of
// a raw array (docs/stl/map_and_unordered_map.md) - useful when values aren't
// small non-negative ints with a known max, so array-hashing doesn't apply.
// No queries needed: the highest frequency in the WHOLE array is found by
// scanning every entry already in the map, not by asking about a few numbers.
int maxFrequency(vector<int>& nums) {
    unordered_map<int, int> ump;

    for (int i = 0; i < (int)nums.size(); i++)
        ump[nums[i]]++;

    int maxFreq = 0;
    for (auto& entry : ump)
        maxFreq = max(maxFreq, entry.second);

    return maxFreq;
}

int main() {
    vector<int> nums = {1, 2, 4, 2, 4, 4};

    int maxVal = maxFrequency(nums);

    cout << maxVal << " is the highest frequency of the number" << endl;

    return 0;
}

// Time Complexity: O(n) - one pass to build the map (n inserts, O(1) average each),
// then one pass over the map's distinct entries (at most n) to find the max.
// Space Complexity: O(n) - the map holds up to n distinct key-value pairs.
