#include <stdio.h>
#include "../headers/substract.h"

int Subtract(int num1, int num2) {

  int subtract;

  // Adding two input numbers
  subtract = num1 - num2;

  printf("From Substract:Subtraction of %d and %d is: %d", num1, num2, subtract);
  return subtract;
}