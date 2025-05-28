#include<stdio.h>

int main() {
	const int a = 40;
	const int* ptr = &a;
	printf("*ptr=%d\n", *ptr);
	
	const int b = 20;
	ptr = &b;
	printf("*ptr=%d\n", *ptr);

	return 0;
}
