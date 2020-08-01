#include <stdio.h>
#include <stdlib.h>
#define number 10

int power(int base, int deg)
{
	int i, out = 1;
	for (i = 0; i < deg; i ++)
	{
		out *= base;
	}
	base = 3;
	return out;
}
void change(int* p, int n)
{
	*p = n;
}

int main(int argc, char const *argv[])
{
	int i = 0, base, deg;
	if (argc != 3)
	{
		printf("Wrong input\n");
		return -1;
	}
	base =  atoi(argv[1]);
	deg  = atoi(argv[2]);
	i = power(base, deg);
	printf("%d^%d = %d\n", base, deg, i);
	change(&base, 5);
	printf("%d\n",base);
	return 0; 
}