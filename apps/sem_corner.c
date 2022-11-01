#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sem.h"
#include "uthread.h"

#define LONGEST 36

#ifndef RETVALS
#define RETVALS
#define RET_SUCCESS 0
#define RET_FAILURE -1
#endif

#define TEST_ASSERT(assert)\
do {\
	int k;\
	for(k = 0; k < LONGEST-format; k++) fprintf(stdout, " ");\
	if (assert) {\
		printf(" ... \033[0;32m[PASS]\033[0m\n");\
	} else	{\
		printf(" ... \033[0;31m[FAIL]\033[0m\n");\
	}\
} while(0)

static sem_t sem;
static int format;

char result[32];
char pattern[13] = "[B][C][C][A]";

static void threadC(void *data)
{
	(void)data;
	sem_down(sem);
	strncat(result, "[C]", 3);
	fprintf(stdout, "[C]\n");
	uthread_yield();
	strncat(result,  "[C]", 3);
	fprintf(stdout, "[C]\n");
	sem_up(sem);
}

static void threadB(void *data)
{
	(void)data;
	sem_up(sem);
	strncat(result, "[B]", 3);
	fprintf(stdout, "[B]\n");
}

static void threadA(void *data)
{
	(void)data;
	sem_down(sem);
	strncat(result, "[A]", 3);
	fprintf(stdout, "[A]\n");
}

void thread1(void *data)
{
	(void)data;
	uthread_create(threadA, NULL);
	uthread_create(threadB, NULL);
	uthread_create(threadC, NULL);

	TEST_ASSERT(strcmp(pattern, result));
	printf("END\n\n");
}

int main(void)
{
	sem = sem_create(0);
	uthread_run(false, thread1, NULL);

	return EXIT_SUCCESS;
}