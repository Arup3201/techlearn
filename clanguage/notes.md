# Notes on C programming

## Pointers

### 1 - Basic of Pointers

Pointer declaration - 
`int *ptr`: a pointer `ptr` pointing to an address containing variable of type `int`.

Pointer initialization - 
```c
int a = 20;
int* ptr;
ptr = &a;
```

Pointer definition -
```c
int a = 20;
int* ptr = &a;
```

Pointer dereferencing -
```c
#include<stdio.h>

int main() {
	int a = 10;
	int* ptr = &a;

	printf("Value at ptr = %p\n", ptr);
	printf("Value at a = %d\n", a);
	printf("Value at *ptr = %d\n", *ptr);

	return 0;
}
```

Output:
```
Value of p=0x7ffe5529741c
Value of a=20
Value of *ptr=20
```

### 2 - pointer and constant

A pointer pointing to a constant integer -
```c
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
```

```
const int a = 40;
const int* ptr = &a;
```

A constant pointer pointing to an integer -
```c
#include<stdio.h>

int main() {
	int a = 40;
	int* const ptr = &a;
	printf("*ptr=%d\n", *ptr);
	
	*ptr = 20;
	printf("*ptr=%d\n", *ptr);

	return 0;
}
```

```
*ptr=40
*ptr=20
```

A constant pointer pointing to a constant integer -
```c
#include<stdio.h>

int main() {
	const int a = 40;
	const int* const ptr = &a;
	
	*ptr = 20;
	const int b = 20;
	ptr = &b;

	return 0;
}
```

```
example02-3.c: In function ‘main’:
example02-3.c:8:14: error: assignment of read-only location ‘*(const int *)ptr’
    8 |         *ptr = 20;
      |              ^
example02-3.c:10:13: error: assignment of read-only variable ‘ptr’
   10 |         ptr = &b;
      |             ^

```
