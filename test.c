#include <stdio.h>

/*void func(int n){
	int arr[n];
	arr[0x200] = 1;
}*/

int main(){
	int arr[10];
	arr[10] = 1;
	int *arr2 = malloc(0x10);
	arr2[4] = 1;

}
/*
	void *ptr = &arr;
	void *ptr2 = ptr;
	*((int *)ptr2 + 10) = 1;
}
*/