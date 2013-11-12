/*
 * Custom Test for Lab Assignment 5
 */
#include <types.h>
#include <lib.h>
#include <test.h>

void sayhello(void* junk, void* morejunk)
{
	(void) junk;
	(void) morejunk;
	kprintf("Hello again, world!\n");
}

int hellotest(int nargs, char **args)
{
	kprintf("Hello, world!\nArgs: ");
	for (int i = 0; i < nargs; i++)
	{
		kprintf(args[i]);
		kprintf(" ");
	}
	kprintf("\n");
	thread_fork("sayhello_thread", NULL, sayhello, NULL, NULL);
	return 0;
}
