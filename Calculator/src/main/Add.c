#include <stdio.h>
#include "../headers/add.h"

int Add(int num1, int num2) {

  int sum;

  // Adding two input numbers
  sum = num1 + num2;

  printf("From Add:Sum of %d and %d is: %d", num1, num2, sum);
  return sum;
}