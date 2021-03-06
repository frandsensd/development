/* ANSI-C implementation for maximum portability. Please configure compiler settings accordingly.
** Notes:
** When built for windows (Visual Studio) output files are 100% identical to test vectors (except for spelling in ref).
** When built for Linux (GCC) output files contains extra CR. Compare using diff --strip-trailing-cr <OUTFILE> <REFFILE>.
*/

#include <stdio.h>
#include <errno.h>	/* Global containing error info */
#include "types.h"
#include "commonDefinitions.h"
#include "usage.h"
#include "readInputData.h"
#include "fifoBuffer.h"
#include "list.h"
#include "writeResults.h"
#include "Fitbit12bitBin2TextUtil.h"

/* main routine - responsible for handling input arguments and main control.
** Arguments:
**  nargs:	Number of arguments including the executable name. Readonly.
**          TWO_ARGUMENTS beyond executable name are required.
**  argv:	Array of strings containing arguments. Readonly.
**			argv[EXECUTABLENAMEID] - executable name
**			argv[INPUTFILENAMEID]  - binary input file
**			argv[OUTPUTFILENAMEID] - ASCII output file
** Return values:
**	OK or error from errorcodes_t.
*/
errorcodes_t main(const int nargs, const char* const argv[])
{
	FILE*			fInFile;
	FILE*			fOutFile;
	union {
		errorcodes_t	common;
		listerrors_t	list;
	} result;

	/* Validate arguments, open binary file and create ASCII file */
	if (nargs != TWO_ARGUMENTS) {
		Usage(argv[EXECUTABLENAMEID]);
		return ERROR_INCORRECTARGS;
	}
	fInFile = fopen(argv[INPUTFILENAMEID], "rb");
	if (NULL == fInFile) {
		printf("%s: Error: Cannot open %s - error: %d", __FUNCTION__, argv[INPUTFILENAMEID], errno);
		return ERROR_CANNOTOPENINPUTFILE;
	}
	fOutFile = fopen(argv[OUTPUTFILENAMEID], "w");
	if (NULL == fOutFile) {
		printf("%s:Error: Cannot create %s - error: %d", __FUNCTION__, argv[OUTPUTFILENAMEID], errno);
		fclose(fInFile);
		return ERROR_CANNOTCREATEOUTPUTFILE;
	}

	/* Initialize internal storage */
	/* FifoBufferInit guaranteed success - FIFOERROR_OK - no check */
	FifoBufferInit();
	/* ListInit needs check */
	if (LISTERROR_MEMORYALLOCATIONFAILED == ListInit()) {
		return LISTERROR_MEMORYALLOCATIONFAILED;
	}

	/* Main loop: Read raw binary data, convert to internal storage width and store */
	do
	{
		result.common = ReadInputData(fInFile);
	} while (!feof(fInFile) && (OK == result.common));

	/* Done with fInFile */
	if (fclose(fInFile)) {
		printf("%s:Error: Failed closing file %s", __FUNCTION__, argv[INPUTFILENAMEID]);
	}

	/* Write output results */
	WriteResults(fOutFile);

	/* Deallocate list */
	if (LISTERROR_OK != (result.list = ListDeInit())) {
		printf("%s:Error: Failed deallocating list - code %d!", __FUNCTION__, result.list);
	}

	/* Done with fOutFile */
	if (fclose(fOutFile)) {
		printf("%s:Error: Failed closing file %s", __FUNCTION__, argv[OUTPUTFILENAMEID]);
	}

	printf("Done processing %s.\n", argv[INPUTFILENAMEID]);

	return OK;
}
