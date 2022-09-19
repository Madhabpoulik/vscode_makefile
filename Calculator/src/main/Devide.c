#include <stdio.h>
#include "../headers/devide.h"

int Devide(int num1, int num2) {

  int res;

  // Adding two input numbers
  res = num1 / num2;

  printf("From Devide:devidation of %d and %d is: %d", num1, num2, res);
  return res;
}