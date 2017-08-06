#ifndef _FITBIT12BITBIN2TEXTUTIL_COMMONDEFINITIONS_H_
#define _FITBIT12BITBIN2TEXTUTIL_COMMONDEFINITIONS_H_

#define BITSPERBYTE				8
#define	BITSPERNIBBLE			4

#define ACCELEROMETERRESOLUTION 12		// Bits/sample from accelerometer.
#define SAMPLESTOSTORE          32		// Amount of samples to store in buffers

/* BITBLOCKSIZEBYTES contains the number of bytes to be read from binary input 
** file per file access */
#define BITBLOCKSIZEBYTES		(ACCELEROMETERRESOLUTION*SAMPLESTOSTORE)/BITSPERBYTE

/* BITBLOCKSIZENIBBLES contains the number of nibbles to be read from binary 
** input file per file access */
#define BITBLOCKSIZENIBBLES		(ACCELEROMETERRESOLUTION*SAMPLESTOSTORE)/BITSPERNIBBLE

/* BYTESPERSAMPLE contains the number of bytes allocated in buffer per accelerometer sample */
#define BYTESPERSAMPLE			2

typedef enum errorcodes_t {
	ERROR_INCORRECTARGS = -8192,
	ERROR_CANNOTOPENINPUTFILE,
	ERROR_CANNOTCREATEOUTPUTFILE,
	ERROR_READFAILED,
	ERROR_LISTACCESSFAILED,
	OK = 0
} errorcodes_t;


#endif /* _FITBIT12BITBIN2TEXTUTIL_COMMONDEFINITIONS_H_ */

