#ifndef _FITBIT12BITBIN2TEXTUTIL_READINPUTDATA_H_
#define _FITBIT12BITBIN2TEXTUTIL_READINPUTDATA_H_

#include <stdio.h>
#include "commonDefinitions.h"

/* ReadInputData reads data from binary file, converts to storage format
** and stores data in internal buffers.
** Arguments:
**  fInFile: File handle for binary input file.
** Return value:
**  OK or error from errorcodes_t.
*/
errorcodes_t ReadInputData(FILE *const fInFile);

#endif /* _FITBIT12BITBIN2TEXTUTIL_READINPUTDATA_H_ */
