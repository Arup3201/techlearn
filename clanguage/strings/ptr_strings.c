#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main() {
	char *str = (char*)malloc(20);
	strcpy(str, "hello");
	printf("%s\n", str);
	return 0;
}
