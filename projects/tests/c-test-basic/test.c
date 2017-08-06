// Build using
// gcc test.c -o launchme

#include <stdio.h>
#include "types.h"

int main (int argc, char* argv[])
{
	int i;
	printf("Hello World\n");
	for (i=0; i<argc; i++)
		printf("arg[%d]=%s\n", i, argv[i]);
	return 0;
}
