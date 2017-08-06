#ifndef _FITBIT12BITBIN2TEXTUTIL_FIFOBUFFER_H_
#define _FITBIT12BITBIN2TEXTUTIL_FIFOBUFFER_H_

#include "types.h"
#include "commonDefinitions.h"

/* fifo_t is a circular buffer.
** readIdx=writeIdx=0 at initialization.
** head is where next sample is to be written. Updated modulo SAMPLESTOSTORE.
** tail is where oldest sample is located. Updated modulo SAMPLESTOSTORE.
** count is amount of samples in buffer. Will not exceed SAMPLESTOSTORE.
** store is the sample storage of size sample_t*SAMPLESTOSTORE.
*/
typedef struct fifo_t {
	uint8_t		head;		/* insertion index modulo SAMPLESTOSTORE */
	uint8_t		tail;		/* read index modulo SAMPLESTOSTORE */
	uint8_t		count;		/* amount of valid samples in buffer */
	sample_t	store[SAMPLESTOSTORE];
} fifo_t;

/* Specific errors to FIFO
*/
typedef enum fifoerrors_t {
	FIFOERROR_OK			=	0,
	FIFOERROR_BUFFEREMPTY	=	0x8100,
	FIFOERROR_BUFFERFULL
} fifoerrors_t;

/* FifoBufferInit initializes the FIFO structure.
** Call this prior to any of below functions.
** Return value:
**  FIFOERROR_OK
*/
fifoerrors_t	FifoBufferInit();

/* FifoBufferGetCount retrieves amount of samples stored in FIFO.
** Return value:
**  uint8_t: Amount of samples  suffices as we only have max 32 values.
*/
uint8_t			FifoBufferGetCount();

/* FifoBufferInsert inserts sample to FIFO and updates management variables.
** Arguments:
**  sample:	Accelerometer sample. Readonly input.
** Return value:
**  FIFOERROR_OK or error from fifoerrors_t.
*/
fifoerrors_t	FifoBufferInsert( const sample_t sample );

/* FifoBufferRead reads sample from FIFO and updates management variables.
** Arguments:
**  sample:	Accelerometer sample retrieved from FIFO(tail). Output.
** Return value:
**  FIFOERROR_OK or error from fifoerrors_t.
*/
fifoerrors_t	FifoBufferRead( sample_t* const sample );

/* FifoBufferResetReadToOldestValue ensures tail is properly adjusted 
** should we have full buffer. Call this function prior to reading out
** data from FIFO.
** Return value:
**  FIFOERROR_OK.
*/
fifoerrors_t	FifoBufferResetReadToOldestValue();

#endif /* _FITBIT12BITBIN2TEXTUTIL_FIFOBUFFER_H_ */

