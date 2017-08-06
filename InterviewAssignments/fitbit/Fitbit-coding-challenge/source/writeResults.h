#ifndef _FITBIT12BITBIN2TEXTUTIL_WRITERESULTS_H_
#define _FITBIT12BITBIN2TEXTUTIL_WRITERESULTS_H_

#include <stdio.h>
#include "commonDefinitions.h"

/* WriteResults writes the output data to output text file per 
** output specification.
** Arguments:
**  fOutFile:				File handle for output file.
** Return value:
**  OK if all is good, otherwise ERROR_WRITETOFILEFAILED.
*/
errorcodes_t WriteResults(FILE *const fOutFile);

#endif /* _FITBIT12BITBIN2TEXTUTIL_WRITERESULTS_H_ */
