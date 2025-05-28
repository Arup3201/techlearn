
#include<stdio.h>

int main() {
	const int a = 40;
	const int* const ptr = &a;
	
	*ptr = 20;
	const int b = 20;
	ptr = &b;

	return 0;
}
