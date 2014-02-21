#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>

#define MAX_THREADS 10 // k<10 threads

// prototypes
void printanum(void*, unsigned int);
void runthreads(int);
void tincrementcounter(void*, int);
void tsemincrementcounter(void*, int);

static int counter = 0;
static struct semaphore *tfsem;
static struct semaphore *tdonesem;
static struct lock* incrementer;
static struct lock* counting;

static void init_testsem(void)
{
	if (tfsem==NULL) {
		tfsem = sem_create("tfsem",2);
		if (tfsem == NULL) {
			panic("threadfun.c: sem_create failed\n");
		}
	}
	if (tdonesem == NULL) {
		tdonesem = sem_create("tdonesem",0);
		if (tdonesem == NULL) {
			panic("threadfun.c: sem_create failed\n");
		}
	}
}

void cleanup_testsem(void)
{
	sem_destroy(tfsem);
	sem_destroy(tdonesem);
}

int unsafethreadcounter(int nargs, char** args)
{
	int i, k, result, numincrements;
	char name[] = "UnsafeThread";

	if (nargs < 3) {
		kprintf("Call with utc <numthreads> <numincrementsperthread>\n");
		return 0;
	}
	k = atoi(args[1]); // k threads
	numincrements = atoi(args[2]); // number of times each thread increments counter

	for (i=1; i <= k; i++)
	{
		result = thread_fork(name, NULL, tincrementcounter, NULL, numincrements);
		if (result) {
			panic("utc: thread_fork failed %s)\n", strerror(result));
		}
	}
	kprintf("Final counter value: %d \n", counter);
	kprintf("UnsafeThreadCounter done.\n");
	counter = 0;
	return 0;
}

int safethreadcounter(int nargs, char** args)
{
	int i, k, result, numincrements;
	char name[] = "SafeThread";
	counting = lock_create("counter_lock");
	incrementer = lock_create("incrementer_lock");

	if (nargs < 3) {
		kprintf("Call with stc <numthreads> <numincrementsperthread>\n");
		return 0;
	}
	k = atoi(args[1]);
	numincrements = atoi(args[2]);
	lock_acquire(incrementer);

	for (i=1; i<=k; i++)
	{
		result = thread_fork(name, NULL, tsemincrementcounter, NULL, numincrements);
		if (result) {
			panic("utc: thread_fork failed %s)\n", strerror(result));
		}
	} 

	lock_release(incrementer);
	lock_acquire(counting);
	lock_acquire(incrementer);
	kprintf("Final counter value: %d \n", counter);
	kprintf("SafeThreadCounter done.\n");
	counter = 0;
	lock_release(counting);
	lock_release(incrementer);
	return 0;
}

void tincrementcounter(void* junk, int numincrements)
{
	int i;
	(void) junk;

	for(i=0; i < numincrements; i++)
		counter++;
}

void tsemincrementcounter(void* junk, int numincrements)
{
	int i;

   lock_acquire(counting);
	kprintf("Entered a critical section!\n");
	for(i=0; i <= numincrements; i++)	 	
		++counter;
	lock_release(counting);
}

void printanum(void *junk, unsigned int num)
{
	(void)junk;
	kprintf("Happy ThreadFun #%d\n",num);
}

void runthreads(int numThreads) {
	int i, result;
	char name[] = "TestThread";

	for (i=0 ; i < numThreads; i++)
	{
		snprintf(name, sizeof(name), "threadfun%d", i);
		result = thread_fork(name, NULL, printanum, NULL, i);
		if(result) {
			panic("threadfun: thread_fork failed %s)\n", strerror(result));
		}
	}
}

int threadfun(int nargs, char **args)
{
	int numThreads;
	if (nargs == 1) 
		numThreads = 10;
	else
		numThreads = atoi(args[1]);
	kprintf("Beginning Thread Fun test...\n");
	runthreads(numThreads);
	kprintf("\nThread Fun test done.\n");

	return 0;
}
