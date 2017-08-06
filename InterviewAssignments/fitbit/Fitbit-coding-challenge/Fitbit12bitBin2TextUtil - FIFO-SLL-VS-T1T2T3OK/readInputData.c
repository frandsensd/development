#include <stdio.h>
#include "types.h"
#include "commonDefinitions.h"
#include "fifoBuffer.h"
#include "list.h"
#include "readInputData.h"

#define ONEBYTE		1

/* storeSamples reads nibbles from the serialized data and writes them to the 
** FIFO storage. Nibble resolution is chosen as input data and desired storage
** size holds 4 bits size as common denominator.
** To be able to control the reading, a nibble counter is used to be able to 
** figure out what we have read from file.
** This nibble counter will be used to map from the serialized data to the FIFO
** sample structure as follows (assuming full buffer):
** BITBLOCKSIZENIBBLES = 96
** SAMPLESTOSTORE = 32
** nibble#		sample#	concatenated value(nibble#)
** 	0,1,2		0		0|1|2
** 	3,4,5		1		3|4|5
**	6,7,8		2		6|7|8
**	...
**  93,94,95	31		93|94|95
** Hence we have:
** sample# = nibble#/3
** sample = nibble#<<8|(nibble#+1)<<4|nibble#+2
** Extracting the nibbles are done simply by alternating the nibbles in the byte
** being considered. The byte being considered is simply every even nibble.
*/
static void storeSamples(const uint8_t *const rawBuffer, const uint8_t bytesRead)
{
	sample_t sample;
	uint8_t nibbleNum;
	uint8_t nibbles = bytesRead * 2;
	uint8_t sampleNum;
	uint8_t samples = nibbles / 3;
	
	for (sampleNum = 0; sampleNum < samples; sampleNum++)
	{
		nibbleNum = sampleNum * 3;
		{
			/* two nibbles per byte. Byte is divided in 
			** {even nibble|odd nibble}. */
			if (nibbleNum & 0x01)
			{
				/* Odd start nibble, get low part of current byte, then full 
				** part of next byte */
				sample = (rawBuffer[nibbleNum/2] & 0x0f) << 8;
				sample |= rawBuffer[nibbleNum/2+1];
			}
			else
			{
				/* Even start nibble, get full part of first byte, then high
				** part of next byte */
				sample = (rawBuffer[nibbleNum/2]) << 4;
				sample |= (rawBuffer[nibbleNum/2+1] & 0xf0) >> 4;
			}
		}
		/* Ignoring error handling here, as Insert is guaranteed to succeed in this
		** implementation */
		fifoBufferInsert(sample);
		listInsert(sample);
	}
}


/* readInputData serves the purpose of reading binary data from file to a raw
** bit buffer, format data for FIFO and store the data herein.
** Arguments:
**  fInFile: Handle for input file.
** Return value:
**  result
*/
errorcodes_t readInputData(FILE *const fInFile)
{
	uint8_t bytesRead;
	uint8_t rawBuffer[BITBLOCKSIZEBYTES];
	
	/* Read raw data to rawBuffer array. No need for large size_t type here */
	bytesRead = (uint8_t) fread(rawBuffer, ONEBYTE, BITBLOCKSIZEBYTES, fInFile);
	printf("%s:bytesRead:%d\n", __FUNCTION__, bytesRead);
	if (BITBLOCKSIZEBYTES != bytesRead) {
		if (!feof(fInFile)) {
			return ERROR_READFAILED;
		}
	}

	/* Format and store raw data to FIFO buffer */
	storeSamples(rawBuffer, bytesRead);

	return OK;
}

