/* Implementation of writeResults per specification in header file.
** Purpose is to write a formatted text output to console explaining user usage.
** Format is specified in header file.
** ANSI-C.
*/

#include <stdio.h>

void usage(const char* executableName)
{
	printf("Usage: %s <inputfilename> <outputfilename>\n", executableName);
	printf("where inputfilename is a binary file containing 12-bit samples.\n");
	printf("      outputfilename is a text file to be created.\n");
}
