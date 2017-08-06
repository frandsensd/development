#ifndef _FITBIT12BITBIN2TEXTUTIL_READINPUTDATA_H_
#define _FITBIT12BITBIN2TEXTUTIL_READINPUTDATA_H_

#include <stdio.h>
#include "types.h"
#include "fifoBuffer.h"

/* readInputData reads data from binary file, converts to storage format
** and stores data in FIFO buffer.
** Arguments:
**  fInFile: File handle for binary input file.
** Return value:
**  Result: OK or error from errorcodes_t.
**
** NOTE: Not returning amount of data read, as this number is useless for this
** application. We just need to know whether we have a full data set or not.
*/
errorcodes_t readInputData(FILE *const fInFile);

#endif /* _FITBIT12BITBIN2TEXTUTIL_READINPUTDATA_H_ */

