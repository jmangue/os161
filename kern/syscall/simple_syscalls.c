#include <types.h>
#include <lib.h>
#include <copyinout.h>
#include <syscall.h>
#include <kern/errno.h>

#define KBUF_MAX 256

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

int sys_write(int fd, const void *buf, size_t nbytes)
{
	char kbuf[KBUF_MAX];
	if (fd != 1)
	{
		kprintf("ERROR: Tried to write to non-console device!\n");
		return EBADF;
	}
	if (nbytes >= KBUF_MAX)
	{
		kprintf("ERROR: tried to write %d bytes into a %d-byte buffer!\n", nbytes, KBUF_MAX);
		return EFAULT;
	}
	copyin((const_userptr_t)buf, &kbuf, nbytes);
	kbuf[nbytes] = '\0';
	return -kprintf(kbuf);
}
