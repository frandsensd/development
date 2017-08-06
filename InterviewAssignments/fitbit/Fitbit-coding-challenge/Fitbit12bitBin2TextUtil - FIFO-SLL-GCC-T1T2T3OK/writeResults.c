#include <stdio.h>
#include "types.h"
#include "fifoBuffer.h"
#include "list.h"
#include "writeResults.h"
#include "commonDefinitions.h"


/* Implementation of writeResults per specification in header file.
** Purpose is to write a formatted text output to a text file, which has been 
** created. Format is specified in header file.
*/
errorcodes_t writeResults(FILE *const fOutFile)
{
	sample_t value;
	uint8_t sample;
	int result;
	uint8_t samples;
	
	listGetLength(&samples);
	/* Will always succeed - ignoring error handling */

	result = fprintf(fOutFile, "--Sorted Max %d Values--\n", samples);
	if (result < 0) {
		printf("%s:Error: Failed writing to output file!\n", __FUNCTION__);
		return result;
	}
	for (sample = 0; sample < samples; sample++)
	{
		if (LISTERROR_OK == listPop(&value)) {
			result = fprintf(fOutFile, "%d\n", value);
			if (result < 0) {
				printf("%s:Error: Failed writing sorted values to output file!\n", __FUNCTION__);
				return result;
			}
		}
		else {
			printf("%s:Error: Failed popping value off list for sample# %d", __FUNCTION__, sample);
		}
	}

	samples = fifoBufferGetCount();
	result = fprintf(fOutFile, "--Last %d Values--\n", samples);
	if (result < 0) {
		printf("%s:Error: Failed writing to output file!\n", __FUNCTION__);
		return result;
	}

	fifoBufferResetReadToOldestValue();
	while ( FIFOERROR_BUFFEREMPTY != fifoBufferRead(&value))
	{
		result = fprintf(fOutFile, "%d\n", value);
		if (result < 0) {
			printf("%s:Error: Failed writing FIFO content to output file!\n", __FUNCTION__);
			return result;
		}
	}

	return OK;
}
