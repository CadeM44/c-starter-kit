/* PROJECTNAME.c */
#include "PROJECTNAME.h"

#include <assert.h>    // assert()
#include <errno.h>     // errno, perror()
#include <stdalign.h>  // alignas, alignof (C11+)
#include <stdatomic.h> // atomic types & ops (C11+)
#include <stdio.h>     // printf, fprintf, etc.
#include <stdlib.h>    // malloc, free, exit, atoi, etc.
#include <string.h>    // memcpy, strcmp, strlen, etc.
#include <threads.h>   // thrd_t, mtx_t, call_once (C11+)

int main(int argc, char* argv[])
{
	printf("Hello World!\n");
	return 0;
}