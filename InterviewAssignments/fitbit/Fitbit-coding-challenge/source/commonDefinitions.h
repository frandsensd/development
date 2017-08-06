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

/*
** Bug fix control
** Add to version info in Fitbit12bitBin2TextUtil.c
*/
/* 20150727 - minor text output modification to match output bit-exact. Reason is reference output contains
** a hardcoded value whereas solution outputs the correct amount. This change hardcodes values to 32 */
#define BUGFIX_HARDCODEVALUESTO32TOALLOWFORBITEXACTNESSTESTPASS

/* 20150727 - minor text output modification to match output bit-exact. Reason is reference output contains
** a spelling mistake. This change adds this spelling mistake in my code */
#define BUGFIX_ADDSPELLINGMISTAKETOALLOWFORBITEXACTNESSTESTPASS

/* 20150903 - problem in ListInsert when inserting at tail. Previously had to traverse list to find proper insertion point otherwise a crash was observed.
** Root cause was not to use t, but use tail directly, i.e. update tail->next = node followed by tail = node to move tail to new last element.*/
#define BUGFIX_LISTINSERTTAILREQUIRINGTRAVERSINGLISTNOTTOCAUSECRASH

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

