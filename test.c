// test.c
#include <stdio.h>
#include <stdlib.h>

int main()
{
  /*
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
  */

  printf("5.\n");
  int *pi = (int *)malloc(sizeof(int));
  printf("pointer: %p\n", pi);
  printf("Address of pi is: %p\n", pi);
  printf("Value of pi is %d\n", *pi);
  // *pi = 20;
  // *(pi + 1) = 10;

  printf("6. error\n");
  int *pi2 = (int *)malloc(sizeof(int) * 10); // 10개의 int 공간 할당
  *pi2 = 1;
  /*
  int *pi_offset = pi2 + 4; // pi에서 4번째 오프셋으로 이동

  printf("untracked\n");
  pi_offset[0] = 42; // 이 시점에서 pi_offset은 추적되지 않음
  */
  return 0;
}
