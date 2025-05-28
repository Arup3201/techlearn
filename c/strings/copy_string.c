#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main() {
	char *dest = (char*)malloc(20);
	char *src = (char*)malloc(20);

	strcpy(src, "hello");
	strcpy(dest, src);

	printf("%s\n", dest);

	return 0;
}
