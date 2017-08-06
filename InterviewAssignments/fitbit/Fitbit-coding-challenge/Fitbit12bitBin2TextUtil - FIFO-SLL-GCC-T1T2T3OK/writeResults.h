/* writeResults writes the output data to output text file.
** Output specification:
**  "--Sorted Max 32 Values--\n"
**  "<32 largest values from inputfile, one per line, ascending order>"
**  "--Last 32 Values--\n"
**  "<32 last values from inputfile, one per line, same order as read>"
**
** Arguments:
**  fOutFile:				File handle for output file.
**  max32SamplesBuffer:		Array containing max 32 samples.
**	last32SamplesBuffer:	Array containing last 32 samples.
**  samples:				Amount of samples in array, typically 32, but may be smaller if input set do not contain 32 samples.
**
** Return value:			OK if all is good, otherwise error code from fprintf routine.
*/

#ifndef _FITBIT12BITBIN2TEXTUTIL_WRITERESULTS_H_
#define _FITBIT12BITBIN2TEXTUTIL_WRITERESULTS_H_

#include <stdio.h>
#include "types.h"
#include "commonDefinitions.h"

errorcodes_t writeResults(FILE *const fOutFile);

#endif /* _FITBIT12BITBIN2TEXTUTIL_WRITERESULTS_H_ */
