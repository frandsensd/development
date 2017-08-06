#include <string.h>		/* memset */
#include <assert.h>
#include "types.h"
#include "commonDefinitions.h"
#include "fifoBuffer.h"

/* Ensure only one instance of buffer accessible only to functions in this file
*/
static fifo_t	buffer;


#define FIFOBUFFER_INITPATTERN	0x0

/* FifoBufferInit initializes the FIFO structure.
** Return value:
**  FIFOERROR_OK
*/
fifoerrors_t FifoBufferInit()
{
	/* NOTE: If multiple threads - need to protect this part! */
	buffer.head = 0;
	buffer.tail = 0;
	buffer.count = 0;
	memset( buffer.store, 
			FIFOBUFFER_INITPATTERN, 
			SAMPLESTOSTORE * BYTESPERSAMPLE );
	return FIFOERROR_OK;
}

/* FifoBufferGetCount retrieves amount of samples stored in FIFO.
** Return value:
**  uint8_t: Amount of samples  suffices as we only have max 32 values.
*/
uint8_t FifoBufferGetCount()
{
	return buffer.count;
}

/* FifoBufferInsert inserts sample to FIFO at head and updates management variables
** head modulo SAMPLESTOSTORE and count limiting at SAMPLESTOSTORE.
** Arguments:
**  sample:	Accelerometer sample. Readonly input.
** Return value:
**  FIFOERROR_OK or error from fifoerrors_t.
*/
fifoerrors_t FifoBufferInsert( const sample_t sample )
{
	/* NOTE: If multiple threads - need to protect this part! */
	/* Special case here: If buffer is full we simply overwrite the 
	** oldest values. Hence no need for full check here and 
	** subsequent error handling. */
	buffer.store[buffer.head] = sample;
	/* NOTE: Special case here for buffer.count as we simply overwrite the older values in this buffer
	** hence ignoring overflow handling on purpose. We just want latest SAMPLESTOSTORE samples in FIFO.
	** Thus buffer.count must not exceed SAMPLESTOSTORE. */
	if (buffer.count < SAMPLESTOSTORE) {
		buffer.count++;
	}
	buffer.head = (buffer.head + 1) % SAMPLESTOSTORE;
	return FIFOERROR_OK;
}

/* FifoBufferRead reads sample from FIFO at tail and updates management variables
** tail modulo SAMPLESTOSTORE and count. Special care is taken to return error when 
** buffer is empty as returned sample is now invalid.
** Arguments:
**  sample:	Accelerometer sample retrieved from FIFO(tail). Output.
** Return value:
**  FIFOERROR_OK or FIFOERROR_BUFFEREMPTY if sample is invalid.
*/
fifoerrors_t FifoBufferRead( sample_t* const sample )
{
	assert(sample != NULL);
	/* NOTE: If multiple threads - need to protect this part! */
	if (0 == buffer.count) {
		return FIFOERROR_BUFFEREMPTY;
	}
	*sample = buffer.store[buffer.tail];
	buffer.tail = (buffer.tail + 1) % SAMPLESTOSTORE;
	buffer.count--;
	return FIFOERROR_OK;
}

/* FifoBufferResetReadToOldestValue ensures we start reading from buffer
** at the oldest value (head) satisfying the requirement of listing data in 
** order read from accelerometer. 
** In case buffer has not reached SAMPLESTOSTORE, tail points at the beginning 
** of the buffer, hence first value read. In this case no need to manipulate 
** tail.
** Call this function prior to reading out data from FIFO.
** Return value:
**  FIFOERROR_OK.
*/
fifoerrors_t	FifoBufferResetReadToOldestValue()
{
	/* reset tail to point to oldest sample in set */
	if (buffer.count == SAMPLESTOSTORE) {
		buffer.tail = buffer.head;
	}
	return FIFOERROR_OK;
}
