#include<iostream>
using namespace std;

int findUnique(int arr[],int size) {
int ans=0;
for(int i=0; i<size; i++) {
    ans = ans^arr[i];

}
return ans;
}


int main() {
int unq[7] ={4,6,3,7,6,3,4};
int result = findUnique(unq, 7);
cout<<result<<endl;

    return 0;
}