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

int sys_printstring(char* s, size_t len)
{
	char s_safe[len];
	size_t string_length;
	copyinstr((const_userptr_t) s, s_safe, len, &string_length);
	kprintf(s_safe);
	return string_length;
}
