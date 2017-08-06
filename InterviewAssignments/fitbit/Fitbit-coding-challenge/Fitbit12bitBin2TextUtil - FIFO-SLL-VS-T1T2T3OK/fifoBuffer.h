#ifndef _FITBIT12BITBIN2TEXTUTIL_FIFOBUFFER_H_
#define _FITBIT12BITBIN2TEXTUTIL_FIFOBUFFER_H_

#include "types.h"
#include "commonDefinitions.h"

/* Consider lastSamplesBuffer a circular buffer.
** readIdx=writeIdx=0 at initialization.
** writeIdx is where next sample is to be written.
** readIdx is where oldest sample is located (FIFO).
** samples is amount of samples in buffer.
*/

typedef struct fifo_t {
	uint8_t		head;		/* insertion index modulo SAMPLESTOSTORE */
	uint8_t		tail;		/* read index modulo SAMPLESTOSTORE */
	uint8_t		count;		/* amount of valid samples in buffer */
	sample_t	store[SAMPLESTOSTORE];
} fifo_t;

typedef enum fifoerrors_t {
	FIFOERROR_OK			=	0,
	FIFOERROR_BUFFEREMPTY	=	0x8000,
	FIFOERROR_BUFFERFULL	=	0x8001
} fifoerrors_t;

uint8_t			fifoBufferGetCount();
fifoerrors_t	fifoBufferInsert( const sample_t sample );
fifoerrors_t	fifoBufferRead( sample_t* const sample );
fifoerrors_t	fifoBufferResetReadToOldestValue();

#endif /* _FITBIT12BITBIN2TEXTUTIL_FIFOBUFFER_H_ */

