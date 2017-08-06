#ifndef _FITBIT12BITBIN2TEXTUTIL_LIST_H_
#define _FITBIT12BITBIN2TEXTUTIL_LIST_H_

#include "types.h"

/* Node definition used in the single linked list.
** sample contains the accelerometer data formatted to sample_t storage.
** next is the forward reference.
*/
typedef struct singlelistnode_t
{
	sample_t					sample;
	struct singlelistnode_t*	next;
} singlelistnode_t;

/* singlelist_t is the list structure implementing the single linked list.
** head points to the smallest value in list (first node).
** tail points to the largest value in list (last node).
** nodes keeps track on amount of nodes allocated in list.
** NOTES: 
** Adding tail helps in inserting value larger than the largest in list,
** hereby avoiding traversing the full list.
** As we don't have to store more than 32 values a uint8_t suffices to 
** store amount of nodes.
*/
typedef struct singlelist_t
{
	singlelistnode_t*	head;
	singlelistnode_t*	tail;
	uint8_t				nodes;
} singlelist_t;

/* Errors specific to single linked list.
*/
typedef enum listerrors_t {
	LISTERROR_OK = 0,
	LISTERROR_LISTNOTINITIALIZED = 0x8200,
	LISTERROR_MEMORYALLOCATIONFAILED,
	LISTERROR_MEMORYDEALLOCATIONFAILED,
	LISTERROR_BUFFEREMPTY,
	LISTERROR_MEMORYNODESIMBALANCE
} listerrors_t;

/* ListInit initializes single linked list structure and allocates first node.
** Call this function prior to any of below functions.
** Return value:
**  LISTERROR_OK or LISTERROR_MEMORYALLOCATIONFAILED
*/
listerrors_t ListInit();

/* ListDeInit deallocates single linked list structure.
** Call this function after end of use.
** Return value:
**  LISTERROR_OK or LISTERROR_MEMORYDEALLOCATIONFAILED
*/
listerrors_t ListDeInit();

/* ListInsert inserts sample in single linked list in ordered manner
** with head always pointing to smallest sample in list and tail always
** pointing to largest sample in list.
** Arguments:
**  sample:	Accelerometer sample to be inserted. Readonly input.
** Return value:
**  LISTERROR_OK or error from listerrors_t.
*/
listerrors_t	ListInsert(const sample_t sample);

/* ListPop retrieves smallest value from list, updates head to point 
** to next node and deletes the obsoleted node.
** Arguments:
**  sample:	Accelerometer sample retrieved from list. Output.
** Return value:
**  LISTERROR_OK or LISTERROR_BUFFEREMPTY.
*/
listerrors_t	ListPop(sample_t *const sample);

/* ListGetLengh retrieves amount of nodes stored in list.
** Arguments:
**  length:	nodes in list. Output.
** Return value:
**  LISTERROR_OK.
*/
listerrors_t	ListGetLength(uint8_t *const length);

#endif /* _FITBIT12BITBIN2TEXTUTIL_LIST_H_ */
