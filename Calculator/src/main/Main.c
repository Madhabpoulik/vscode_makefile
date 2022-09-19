#include <stdio.h>
#include "../headers/add.h"
#include "../headers/multiply.h"
#include "../headers/substract.h"
#include "../headers/devide.h"

int main(int argc, char *argv[])
{
	int a,b, result;
	char opr;
	
	if(argc!=4)
	{
		printf("Invalid arguments...\n");
		return -1;
	}
	
	//get values
	char *x = malloc(100);
	a = atoi(argv[1]);
	b = atoi(argv[3]);
	
	//get operator
	opr=argv[2][0];
	
	//calculate according to operator
	switch(opr)
	{
		case '+':
			result=Add(a,b);
			break;
		case '-':
			result=Subtract(a,b);
			break;
		case 'x':
			result=Multiply(a,b);
			break;
        case '/':
			result=Devide(a,b);
			break;
		default:
			result=0;
			break;
	}
	
	if(opr=='+' || opr=='-' || opr=='x' || opr== '/')
		printf("\nResult: %d %c %d = %d\n",a,opr,b,result);
	else
		printf("Undefined Operator...\n");
	
	return 0;
}
