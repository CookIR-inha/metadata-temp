// test.c
#include <stdio.h>
#include <stdlib.h>

int global;


int arr_global[100];
int func(int n){
  int arr[n];
}

int main()
{
  printf("1. malloc.\n");
  int arr3[0x100];
  int *arr = (int *)malloc(10 * sizeof(int));
  int *arr2 = (int *)malloc(10 * sizeof(int));
  int n = 44;
  char a = 1;
  short b = 1;

  char arr4[n-1];
  printf("2. Allocated pointer: %p\n", arr);
  // 일반적인 메모리 접근
  printf("3. Accessing allocated memory\n");
  for (int i = 0; i < 10; i++)
  {
    arr[i] = i * 10;
    printf("pointer: %p\n", (void *)&arr[i]);
  }

  printf("Accessing arr[5]: %d\n", arr[5]);
  func(4);
  printf("4. Accessing out-of-bound.\n");
  // 메모리 범위를 넘는 접근
  printf("input index : ");
  int index = 0x44;
  scanf("%d",&index);
  arr[index] = 100;
  arr_global[36] = 100;
  free(arr);

  printf("5.\n");
  int *pi = (int *)malloc(sizeof(int));
  printf("pointer: %p\n", pi);
  *pi = 20;
  *(pi + 1) = 10;

  return 0;
}
