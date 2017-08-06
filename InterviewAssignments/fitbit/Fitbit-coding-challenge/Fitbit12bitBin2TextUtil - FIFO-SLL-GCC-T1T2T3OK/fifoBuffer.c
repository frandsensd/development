#include <string.h>		/* memset */
#include "types.h"
#include "commonDefinitions.h"
#include "fifoBuffer.h"

#define FIFOBUFFER_INITPATTERN	0x0
/* Ensure only one instance of buffer accessible only to functions in this file
*/
static fifo_t	buffer;
static uint8_t	isInitialized = FALSE;	

static fifoerrors_t fifoBufferInit()
{
	/* NOTE: If multiple threads - need to protect this part! */
	buffer.head = 0;
	buffer.tail = 0;
	buffer.count = 0;
	memset( buffer.store, 
			FIFOBUFFER_INITPATTERN, 
			SAMPLESTOSTORE * BYTESPERSAMPLE );
	isInitialized = TRUE;
	return FIFOERROR_OK;
}

/* No result return value needed as function cannot fail
*/
uint8_t fifoBufferGetCount()
{
	if (FALSE == isInitialized) {
		fifoBufferInit();
	}
	return buffer.count;
}

/* Insert sample into FIFO buffer and update buffer management
*/
fifoerrors_t fifoBufferInsert( const sample_t sample )
{
	if (FALSE == isInitialized) {
		fifoBufferInit();
	}
	/* NOTE: If multiple threads - need to protect this part! */
	/* Special case here: If buffer is full we simply overwrite the 
	** oldest values. Hence no need for this check.
	** This also implies guaranteed success! */
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

/* Retrieve sample from FIFO buffer and update buffer management
*/
fifoerrors_t fifoBufferRead( sample_t* const sample )
{
	if (FALSE == isInitialized) {
		fifoBufferInit();
	}
	/* NOTE: If multiple threads - need to protect this part! */
	if (0 == buffer.count) {
		return FIFOERROR_BUFFEREMPTY;
	}
	*sample = buffer.store[buffer.tail];
	buffer.tail = (buffer.tail + 1) % SAMPLESTOSTORE;
	buffer.count--;
	return FIFOERROR_OK;
}

/* fifoBufferResetReadToOldestValue ensures we start reading from buffer
** at the oldest value satisfying the requirement of listing data in 
** order read from accelerometer.
*/
fifoerrors_t	fifoBufferResetReadToOldestValue()
{
	/* reset tail to point to oldest sample in set */
	if (buffer.count == SAMPLESTOSTORE) {
		buffer.tail = buffer.head;
	}
	return FIFOERROR_OK;
}
