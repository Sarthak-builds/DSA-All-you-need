#include<iostream>
using namespace std;

void printArray(int arr[], int n) {
    for(int i=0; i<n; i++){
        cout << arr[i] <<" ";
    } cout << endl;
}

void swapAlt(int arr[], int size) {
    for(int i=0; i<size; i+=2){
      if(i+1<size){
        swap(arr[i],arr[i+1]);
      }  
    }
}

int main() {

    int even[8]={5,6,3,78,3,8,4,3};
    int odd[9]= {4,7,3,2,67,3,8,3,8};
    swapAlt(odd,9);
    printArray(odd,9);


    return 0;
}
