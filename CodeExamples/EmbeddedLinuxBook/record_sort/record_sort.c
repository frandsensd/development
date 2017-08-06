/*
* author Doug Abbott
*
* Simple demonstration of building and running a project under Eclipse.
*
* Usage:
*
record_sort <filename> [1 | 2]
*
* Sorts records in <filename> either by name (1) or ID (2).
* Default is name. Outputs sorted file to stdout.
*/
#include <stdio.h>
#include <stdlib.h>
#include "record_sort.h"

int main (int argc, char **argv)
{
	int size, sort = 1;
	record_t *records;

	if (read_file (argv[1], &size, &records))
	{
		printf ("Couldn't open file %s\n", argv[1]);
		exit (1);
	}
	if (argc > 2)
		sort = atoi (argv[2]);

	switch (sort)
	{
		case 1: sort_name (size, records);
				break;
		case 2: sort_ID (size, records);
				break;
		default:
			printf ("Invalid sort argument\n");
			return_records (size, records);
			exit (2);
	}
	write_sorted (size, records);
	return_records (size, records);
	return 0;
}


