#include <stdio.h>

void func(int *arr){
	void *test = arr;
	*((int *)test +12) = 0xff;
}

int main(){
	int arr[10];
	void *arr_copy = arr;
	*((int *)arr_copy + 10) = 1;

	func(arr);
}
/*
	void *ptr = &arr;
	void *ptr2 = ptr;
	*((int *)ptr2 + 10) = 1;
}
*/