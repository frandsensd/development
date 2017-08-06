#ifndef _FITBIT12BITBIN2TEXTUTIL_COMMONDEFINITIONS_H_
#define _FITBIT12BITBIN2TEXTUTIL_COMMONDEFINITIONS_H_

#define BITSPERBYTE				8
#define	BITSPERNIBBLE			4

/* Bits/sample from accelerometer */
#define ACCELEROMETERRESOLUTION 12

/* Amount of samples to store in buffers */
#define SAMPLESTOSTORE          32

/* BITBLOCKSIZEBYTES contains the number of bytes to be read from binary input 
** file per file access */
#define BITBLOCKSIZEBYTES		(ACCELEROMETERRESOLUTION*SAMPLESTOSTORE)/BITSPERBYTE

/* BITBLOCKSIZENIBBLES contains the number of nibbles to be read from binary 
** input file per file access */
#define BITBLOCKSIZENIBBLES		(ACCELEROMETERRESOLUTION*SAMPLESTOSTORE)/BITSPERNIBBLE

/* BYTESPERSAMPLE contains the number of bytes allocated in buffer per accelerometer sample */
#define BYTESPERSAMPLE			2

/* Common error codes.
** NOTE: Specific error codes has different offset.
*/
typedef enum errorcodes_t {
	ERROR_INCORRECTARGS = 0x8000,
	ERROR_CANNOTOPENINPUTFILE,
	ERROR_CANNOTCREATEOUTPUTFILE,
	ERROR_READFAILED,
	ERROR_WRITETOFILEFAILED,
	ERROR_MEMORYALLOCATIONFAILED,
	OK = 0
} errorcodes_t;

/* Useful logging for debug purpose */
#define DBG(...)
/*
#ifdef NDEBUG
#define DBG(...)
#else
#define DBG(...)	printf(__VA_ARGS__)
#endif
*/

#endif /* _FITBIT12BITBIN2TEXTUTIL_COMMONDEFINITIONS_H_ */

