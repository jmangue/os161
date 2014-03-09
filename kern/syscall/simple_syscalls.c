#include <types.h>
#include <lib.h>
#include <copyinout.h>
#include <syscall.h>

int sys_helloworld(void)
{
	return kprintf("Hello, world!\n");
}

int sys__exit(int code)
{
	thread_exit();
	return 0;
}

int sys_printint(int i)
{
	return kprintf("%d",i);
}
