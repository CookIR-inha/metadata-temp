#include <stdio.h>

<<<<<<< HEAD
int main()
{

  printf("1. malloc.\n");
  int *arr = (int *)malloc(10 * sizeof(int));
  int *arr2 = (int *)malloc(10 * sizeof(int));
  printf("2. Allocated pointer: %p\n", arr);
  // 일반적인 메모리 접근
  printf("3. Accessing allocated memory\n");
  for (int i = 0; i < 10; i++)
  {
    arr[i] = i * 10;
    printf("pointer: %p\n", (void *)&arr[i]);
  }
  /*
    printf("Accessing arr[5]: %d\n", arr[5]);
*/
  printf("4. Accessing out-of-bound.\n");
  // 메모리 범위를 넘는 접근
  printf("input index : ");
  int index = 0x44;
  scanf("%d", &index);
  arr[index] = 100;

  free(arr);

  printf("5.\n");
  int *pi = (int *)malloc(sizeof(int));
  printf("pointer: %p\n", pi);
  *pi = 20;
  printf("pointer: %p\n", pi + 1);
  *(pi + 1) = 10;

  return 0;
=======
/*void func(int n){
	int arr[n];
	arr[0x200] = 1;
}*/

int main(){
	int arr[10];
	arr[33] = 1;
	int *arr2 = malloc(0x10);
	arr2[4] = 1;

>>>>>>> simple-detect
}
/*
	void *ptr = &arr;
	void *ptr2 = ptr;
	*((int *)ptr2 + 10) = 1;
}
*/