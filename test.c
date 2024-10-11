// test.c
#include <stdio.h>
#include <stdlib.h>

int main()
{
  printf("1.\n");
  printf("2.\n");
  int *arr = (int *)malloc(10 * sizeof(int));
  printf("pointer: %p\n", arr);
  printf("3.\n");
  // 일반적인 메모리 접근
  for (int i = 0; i < 10; i++)
  {
    arr[i] = i * 10;
    printf("pointer: %p\n", (void *)&arr[i]);
  }

  printf("Accessing arr[5]: %d\n", arr[5]);

  printf("4.\n");
  // 메모리 범위를 넘는 접근
  arr[10] = 100;

  free(arr);

  printf("5.\n");
  int *pi = (int *)malloc(sizeof(int));
  printf("pointer: %p\n", pi);
  *pi = 20;
  *(pi + 1) = 10;

  return 0;
}
