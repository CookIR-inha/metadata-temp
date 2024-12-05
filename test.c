#include <stdio.h>
#include <stdlib.h>
void func(int *arr)
{
	void *test = arr;
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
	// void *arr_copy = arr;
	// *((int *)arr_copy + 11) = 1;
	struct teststruct *node = (struct teststruct *)malloc(sizeof(struct teststruct));
	node->field1 = 1;
	// func(arr);
	int *out_of_bound_access = (int *)(node + 1); // 구조체의 끝을 넘어가는 포인터
	*out_of_bound_access = 0xdeadbeef;			  // 경계 외부 메모리에 쓰기
}
/*
	void *ptr = &arr;
	void *ptr2 = ptr;
	*((int *)ptr2 + 10) = 1;
}
*/