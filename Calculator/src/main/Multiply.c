#include <stdio.h>
#include "../headers/multiply.h"

int Multiply(int num1, int num2) {

  int multi;

  // Adding two input numbers
  multi = num1 * num2;

  printf("From Multiply:multiplication of %d and %d is: %d", num1, num2, multi);
  return multi;
}