#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

// void func(int *arr, int *arr2)
// {
// 	void *test = arr2;
// 	*((int *)test + 11) = 0xff;
// }

// int arr[10];
// int arr2[8];
// void *arr_copy = arr;
// *((int *)arr_copy + 11) = 1;

// func(arr, arr2);


// int *out_of_bound_access = (int *)(node + 1); // 구조체의 끝을 넘어가는 포인터
// *out_of_bound_access = 0xdeadbeef;			  // 경계 외부 메모리에 쓰기
struct teststruct
{
	char field1[4];
	int field2;
	int field3;
};
int main()
{
	// struct teststruct *node = (struct teststruct *)malloc(sizeof(struct teststruct));
	// free(node);
	// node->field2 = 2;
	struct teststruct node;
	node.field2 = 1234;
	node.field3 = 5678;
	strcpy(node.field1, "abcdefg");
	printf("field1: %s\n", node.field1); // Overflow로 인해 field1과 field2가 연결됨
    printf("field2: %d\n", node.field2); // field2의 값이 덮어씌워짐
    printf("field3: %d\n", node.field3); // field3은 영향을 받지 않음
}

/*
	void *ptr = &arr;
	void *ptr2 = ptr;
	*((int *)ptr2 + 10) = 1;
}
*/
