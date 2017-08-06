#ifndef _FITBIT12BITBIN2TEXTUTIL_LIST_H_
#define _FITBIT12BITBIN2TEXTUTIL_LIST_H_

#include "types.h"

typedef struct singlelistnode_t
{
	sample_t					sample;
	struct singlelistnode_t*	next;
} singlelistnode_t;

typedef struct singlelist_t
{
	singlelistnode_t*	head;	/* points to smallest value node in list */
	singlelistnode_t*	tail;	/* points to largest value node in list */
	uint8_t				nodes;
} singlelist_t;

typedef enum listerrors_t {
	LISTERROR_OK = 0,
	LISTERROR_LISTNOTINITIALIZED = 0x8000,
	LISTERROR_MEMORYALLOCATIONFAILED,
	LISTERROR_BUFFEREMPTY
} listerrors_t;

/* listInsert inserts sample in single linked list in ordered manner
** with head always pointing to smallest sample in list.
*/
listerrors_t	listInsert(const sample_t sample);

/* listPop retrieves smallest value from list, updates head to point 
** to next node and deletes the obsoleted node.
*/
listerrors_t	listPop(sample_t *const sample);

/* listGetLengh retrieves amount of nodes stored in list.
*/
listerrors_t	listGetLength(uint8_t *const length);

#endif /* _FITBIT12BITBIN2TEXTUTIL_LIST_H_ */
