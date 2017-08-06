#include <stdio.h>
#include <string.h>	/* memset */
#include <errno.h>	/* Global containing error info */
#include "types.h"
#include "commonDefinitions.h"
#include "usage.h"
#include "fifoBuffer.h"
#include "readInputData.h"
/*
#include "sortData.h"
*/
#include "writeResults.h"
#include "Fitbit12bitBin2TextUtil.h"


/* main routine - responsible for handling input arguments and main control.
** 
*/
errorcodes_t main(const int nargs, const char* argv[])
{
	FILE*			fInFile;
	FILE*			fOutFile;
	errorcodes_t	result;

	/* Validate arguments */
	if (nargs != TWO_ARGUMENTS) {
		usage(argv[EXECUTABLENAMEID]);
		return ERROR_INCORRECTARGS;
	}
	fInFile = fopen(argv[INPUTFILENAMEID], "rb");
	if (NULL == fInFile) {
		printf("Error: Cannot open %s - error: %d", 
			argv[INPUTFILENAMEID], errno);
		return ERROR_CANNOTOPENINPUTFILE;
	}
	fOutFile = fopen(argv[OUTPUTFILENAMEID], "w");
	if (NULL == fOutFile) {
		printf("Error: Cannot create %s - error: %d", 
			argv[OUTPUTFILENAMEID], errno);
		fclose(fInFile);
		return ERROR_CANNOTCREATEOUTPUTFILE;
	}

	/* Main loop: Read data, store in buffer and sort data */
	do
	{
		/* Read block from file, reformat to processing format and store in
		** FIFO and possibly maxSamplesBuffer if condition is met */
		result = readInputData(fInFile);
		
	} while (!feof(fInFile) && (OK == result));

	/* Done with fInFile */
	if (fclose(fInFile)) {
		printf("Note: Failed closing file %s", argv[INPUTFILENAMEID]);
	}

	/* Write output results */
	writeResults(fOutFile);

	/* Done with fOutFile */
	if (fclose(fOutFile)) {
		printf("Note: Failed closing file %s", argv[OUTPUTFILENAMEID]);
	}

	printf("Done processing %s.\n%s contains info for %d samples\n", 
		argv[INPUTFILENAMEID], argv[OUTPUTFILENAMEID], fifoBufferGetCount());

	return OK;
}


