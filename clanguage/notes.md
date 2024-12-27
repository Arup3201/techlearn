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

### Char pointers

If we are using a character pointer then we have to be careful when copying any string to it.

Pointers to char are not initialized, so we try to copy any string with `strcpy` or `strncpy` then it will give a segmentation fault if we are copying it to an uninitialized pointer.

```c
// Wrong method: Segmentation Fault
char* protocol;
strcpy(protocol, "HTTP/1.1");
```

Make sure to initialize the pointer or assign some block of memory before doing it.

```c
char* protocol;
protocol = (char*)malloc(10);
strcpy(protocol, "HTTP/1.1");
```

## Using `memset` to reset address

We can use `memset` from `string.h` to set values for a range of address.

`memset(void* buffer, int value, size_t len)`: It will set the address starting from `buffer` and ending at `buffer+len*sizeof(data_type)` with the value `value`.

## Convert string to integer

Use `atoi` to convert string to integer.

Example:

```c
// ./server 127.0.0.1 8080
#include<stdio.h>

int main(int argc, char* argv[]) {
	int address = argv[1];
	int port = atoi(argv[2]);
}
```

## Build a static library

First create `.h` files which contain a bunch of structures, enums and function declarations. Then create `.c` files that implements each functions declared in the `.h` file.

Lets say I am creating a library header `mylib.h`, which has corresponding `mylib.c`, which implements the `hello` function declared in the header.

After that we have to compile the `.c` files -

```
gcc -c *.c
```

This will compile all the `.c` file of the library folder `lib`.

After you got the object files `.o`, we can store all of those object files in an archieve `.a` file.

```
ar -rc libmy.a *.o
```

Make sure to use `lib` before your library archieve name in this case `libmy.a`.

After this, you can link and compile your file with this library using the following command -

```
gcc main.c -L./lib -lmy -o main
```

You can see we are linking the library with `-L` and then also passing the path where we will find the `.h` file. Using the `-lmy` because the archieve name is `libmy`, that is why giving `lib` in the name is important to make it work.

Also make sure you put your file that you want to compile before the `-L` statement otherwise it will give you `reference error`.
