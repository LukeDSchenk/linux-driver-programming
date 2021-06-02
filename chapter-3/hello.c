#include <stdio.h>

/*
 * Want to see what system calls are made by printf()?
 * Compile this program using:
 * gcc -Wall -o hello hello.c
 * and run strace ./hello
*/

int main(void) {
	printf("hello");
	return 0;
}
