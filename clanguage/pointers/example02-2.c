#include<stdio.h>

int main() {
	int a = 40;
	int* const ptr = &a;
	printf("*ptr=%d\n", *ptr);
	
	*ptr = 20;
	printf("*ptr=%d\n", *ptr);

	return 0;
}
