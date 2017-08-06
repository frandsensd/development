#include <stdio.h>

/* Usage dumps usage info to console.
** Arguments:
**  executableName[]:	Char array containing the first element from argv, the
**                      executable name.
*/
void Usage(const char* executableName)
{
	printf("Usage: %s <inputfilename> <outputfilename>\n", executableName);
	printf("where inputfilename is a binary file containing 12-bit samples.\n");
	printf("      outputfilename is a text file to be created.\n");
}
