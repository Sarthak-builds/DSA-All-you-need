#include <iostream>
using namespace std;

void reverse(int arr[], int n) {
    int start = 0;
    int end = n - 1;
    
    while (start <= end) {
        // manual swap
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        
        start++;
        end--;
    }
}

void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    int arr[6] = { 2, 7, 5, 9, -2, 15 };
    int brr[5] = { 2, 7, 5, 9, -2 };
    
    reverse(arr, 6);
    reverse(brr, 5);
    
    printArray(arr, 6);
    printArray(brr, 5);
    
    return 0;
}