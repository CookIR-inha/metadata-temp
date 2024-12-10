#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
void func(int *arr, int *arr2)
{
	void *test = arr2;
	*((int *)test + 10) = 0xff;
}

struct teststruct
{
	int field1;
	int field2;
	int field3;
};

int main()
{
	int arr[10];
	int arr2[8];
	// void *arr_copy = arr;
	// *((int *)arr_copy + 11) = 1;
	struct teststruct *node = (struct teststruct *)malloc(sizeof(struct teststruct));
	int *value = (int *)((uintptr_t)node - 0x8);
	size_t size = *value & ~0x7;
	node->field1 = 1;
	func(arr, arr2);
	int *out_of_bound_access = (int *)(node + 1); // 구조체의 끝을 넘어가는 포인터
	*out_of_bound_access = 0xdeadbeef;			  // 경계 외부 메모리에 쓰기
}
/*
	void *ptr = &arr;
	void *ptr2 = ptr;
	*((int *)ptr2 + 10) = 1;
}
*/