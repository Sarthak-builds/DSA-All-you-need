

def two_sum_unsorted(arr, target):
    low, high = 0, len(arr) - 1
    while low <= high:
        result = arr[low] + arr[high]
        if result == target:
            return low, high
        if result < target:
            low +=1
        else :
            high -=1
    return None

a = [3,6,7,2,10,56]
sorted = two_sum_unsorted(a, 112)
print(sorted)            