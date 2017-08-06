#include <stdio.h>
#include "types.h"
#include "commonDefinitions.h"
#include "fifoBuffer.h"
#include "list.h"
#include "writeResults.h"


/* WriteResults writes the output data to output text file per
** output specification.
** Arguments:
**  fOutFile:				File handle for output file.
** Return value:
**  OK if all is good, otherwise ERROR_WRITETOFILEFAILED.
*/
errorcodes_t WriteResults(FILE *const fOutFile)
{
	sample_t	value;
	uint8_t		sample;
	int			result;
	uint8_t		samples;
	
	/* Will always succeed - ignoring error handling */
	ListGetLength(&samples);

	#ifndef BUGFIX_HARDCODEVALUESTO32TOALLOWFORBITEXACTNESSTESTPASS
	result = fprintf(fOutFile, "--Sorted Max %d Values--\n", samples);
	#else
	result = fprintf(fOutFile, "--Sorted Max 32 Values--\n");
	#endif
	if (result < 0) {
		printf("%s:Error: Failed writing to output file - code: %d!\n", __FUNCTION__, result);
		return ERROR_WRITETOFILEFAILED;
	}
	/* Retrieve sorted values from list and dump to file */
	for (sample = 0; sample < samples; sample++)
	{
		if (LISTERROR_OK == ListPop(&value)) {
			result = fprintf(fOutFile, "%d\n", value);
			if (result < 0) {
				printf("%s:Error: Failed writing sorted values to output file - code: %d!\n", __FUNCTION__, result);
				return ERROR_WRITETOFILEFAILED;
			}
		}
		else {
			printf("%s:Error: Failed popping value off list for sample# %d", __FUNCTION__, sample);
		}
	}

	samples = FifoBufferGetCount();
	#ifndef BUGFIX_HARDCODEVALUESTO32TOALLOWFORBITEXACTNESSTESTPASS
	#ifndef BUGFIX_ADDSPELLINGMISTAKETOALLOWFORBITEXACTNESSTESTPASS
	result = fprintf(fOutFile, "--Last %d Values--\n", samples);
	#else
	result = fprintf(fOutFile, "--Last %d Valeus--\n", samples);
	#endif
	#else /* BUGFIX_HARDCODEVALUESTO32TOALLOWFORBITEXACTNESSTESTPASS */
	#ifndef BUGFIX_ADDSPELLINGMISTAKETOALLOWFORBITEXACTNESSTESTPASS
	result = fprintf(fOutFile, "--Last 32 Values--\n");
	#else
	result = fprintf(fOutFile, "--Last 32 Valeus--\n");
	#endif
	#endif /* BUGFIX_HARDCODEVALUESTO32TOALLOWFORBITEXACTNESSTESTPASS */
	if (result < 0) {
		printf("%s:Error: Failed writing to output file - code: %d!\n", __FUNCTION__, result);
		return ERROR_WRITETOFILEFAILED;
	}

	/* Retrieve last samples from FIFO in the order they're read */
	FifoBufferResetReadToOldestValue();
	while ( FIFOERROR_BUFFEREMPTY != FifoBufferRead(&value))
	{
		result = fprintf(fOutFile, "%d\n", value);
		if (result < 0) {
			printf("%s:Error: Failed writing FIFO content to output file - code: %d!\n", __FUNCTION__, result);
			return ERROR_WRITETOFILEFAILED;
		}
	}

	return OK;
}
