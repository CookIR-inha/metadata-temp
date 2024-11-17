#include <stdio.h>

/*void func(int n){
	int arr[n];
	arr[0x200] = 1;
}*/

int main(){
	int arr[10];
	arr[11] = 1;

	void *ptr = &arr;
	void *ptr2 = ptr;
	*((int *)ptr2 + 12) = 1;
}