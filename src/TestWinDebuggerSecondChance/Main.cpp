#include <stdlib.h>

void main() {
	// STATUS_INTEGER_DIVIDE_BY_ZERO
	// volatile is to prevent code elimination by the compiler under Release build
	volatile int a = 1 / atoi("0");
}