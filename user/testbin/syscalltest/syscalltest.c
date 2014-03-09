#include <unistd.h>
#include <stdio.h>

void
main(int argc, char *argv[])
{
	helloworld();
	printint(42);
	printstring("\nHere is a sample string!", 26);
	// putchar('Z');
	char z = 'Z';
	printstring("\nWriting Z using write:  ", 26);
	write(1, &z, 1);

	printstring("\nWriting Z using putchar:  ", 28);
	putchar('Z');
}

