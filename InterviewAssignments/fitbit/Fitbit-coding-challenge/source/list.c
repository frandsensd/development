#include <stdio.h>
#include <stdlib.h>	/* malloc/free */
#include <assert.h>
#include "types.h"
#include "commonDefinitions.h"
#include "list.h"

/* Ensure only one instance of list accessible only to functions in this file
*/
static singlelist_t	list;

/* ListInit initializes single linked list structure and allocates first node.
** Call this function prior to any of below functions.
** Return value:
**  LISTERROR_OK or LISTERROR_MEMORYALLOCATIONFAILED
*/
listerrors_t ListInit()
{
	singlelistnode_t*	node;
	node = (singlelistnode_t*)malloc(sizeof(singlelistnode_t));
	if (NULL == node) {
		return LISTERROR_MEMORYALLOCATIONFAILED;
	}
	list.head = node;
	list.tail = node;
	list.nodes = 0;
	return LISTERROR_OK;
}

/* ListDeInit deallocates single linked list structure.
** Call this function after end of use.
** Return value:
**  LISTERROR_OK or LISTERROR_MEMORYDEALLOCATIONFAILED
*/
listerrors_t ListDeInit()
{
	singlelistnode_t*	t = list.head;
	if (NULL == t) {
		if (0 == list.nodes) {
			return LISTERROR_OK;
		}
		else {
			return LISTERROR_MEMORYNODESIMBALANCE;
		}
	}

	/* traverse list from head and delete all remaining nodes */
	do
	{
		singlelistnode_t*	nodeToDelete = t;
		t = t->next;
		free(nodeToDelete);
		list.nodes--;
	} while (t != NULL);
	return LISTERROR_OK;
}

/* ListInsert inserts sample in single linked list in ordered manner
** with head always pointing to smallest sample in list and tail always
** pointing to largest sample in list.
** Arguments:
**  sample:	Accelerometer sample to be inserted. Readonly input.
** Return value:
**  LISTERROR_OK or error from listerrors_t.
*/
listerrors_t	ListInsert(const sample_t newSample)
{
	singlelistnode_t*	node;
	/* At beginning, dummy node is allocated by init - simply fill in data and reuse this node */
	if (0 == list.nodes) {
		node = list.head;
		node->next = NULL;
		node->sample = newSample;
		list.nodes++;
		DBG("%s:Added first sample(%d) to list\n", __FUNCTION__, newSample);
		return LISTERROR_OK;
	}
	else {
		/* Check if we need to create a new node for insertion.
		** - if list is not full we need to allocate new node.
		** - if list is full only if newSample > smallest value in list */
		if (list.nodes < SAMPLESTOSTORE || newSample > list.head->sample) {
			node = (singlelistnode_t*)malloc(sizeof(singlelistnode_t));
			if (NULL == node) {
				return LISTERROR_MEMORYALLOCATIONFAILED;
			}
			node->sample = newSample;
		}
		else {
			DBG("%s:No new node created for sample %d\n", __FUNCTION__, newSample);
			return LISTERROR_OK;
		}
	}

	/* Node created, now insert */
	if (newSample <= list.head->sample && SAMPLESTOSTORE > list.nodes) {
		/* list not full yet and newSample is smaller than or equal to smallest in list - insert at head node */
		node->next = list.head;
		list.head = node;
		list.nodes++;
		DBG("%s: sample %d inserted as first node in list - nodes = %d.\n", __FUNCTION__, newSample, list.nodes);
	}
	else if (newSample >= list.tail->sample) {
		singlelistnode_t* t = list.head;
		/* newSample is larger than the largest sample in list - insert at tail node */
#ifdef BUGFIX_LISTINSERTTAILREQUIRINGTRAVERSINGLISTNOTTOCAUSECRASH // ORIGINAL - not using tail but traversing list..
		/* Current state:
		tail -> [largestvalueInList, NULL]
		node =  [newLargestValue, uninitialized]
		** New state:
		tail->next -> node =  [newLargestValue, NULL]
		tail = node
		*/
		node->next = NULL; // Node will be last element in list
		list.tail->next = node; // previous last element next pointing to node = new last element.
		list.tail = node; // update tail to point to new last element in list.
#else // ORIGINAL CODE:
		while (t->next != NULL) {
			t = t->next;
		}
		node->next = NULL;
		t->next = node;
		list.tail = node;
#endif
		/* Check if list is full - we need to keep largest values, hence we discard the smallest one */
		if (SAMPLESTOSTORE == list.nodes) {
			t = list.head;
			list.head = list.head->next;
			free(t);
			/* No changes to nodes in list */
		}
		else {
			list.nodes++;
		}
		DBG("%s: sample %d inserted as last node in list - nodes = %d, range [%d;%d]\n", 
			__FUNCTION__, newSample, list.nodes, list.head->sample, list.tail->sample );
	}
	else {
		/* newSample is between min/max in list - locate insertion point and insert */
		singlelistnode_t* t = list.head->next;
		singlelistnode_t* prev = list.head;
		while (newSample > t->sample) {
			prev = t;
			t = t->next;
		}
		node->next = t;
		prev->next = node;

		/* Check if list is full - we need to keep largest values, hence we discard the smallest one */
		if (SAMPLESTOSTORE == list.nodes) {
			t = list.head;
			list.head = list.head->next;
			free(t);
			/* No changes to nodes in list */
		}
		else {
			list.nodes++;
		}
		DBG("%s: sample %d inserted in list - nodes = %d, range [%d;%d]\n", 
			__FUNCTION__, newSample, list.nodes, list.head->sample, list.tail->sample);
	}
	return LISTERROR_OK;
}

/* ListPop retrieves smallest value from list(node head points at), 
** updates head to point to next node and deletes the obsoleted node.
** Arguments:
**  sample:	Accelerometer sample retrieved from list. Output.
** Return value:
**  LISTERROR_OK or LISTERROR_BUFFEREMPTY.
*/
listerrors_t	ListPop(sample_t *const sample)
{
	assert(sample != 0);
	if (0 == list.nodes) {
		return LISTERROR_BUFFEREMPTY;
	}
	singlelistnode_t* nodeToPop = list.head;
	*sample = nodeToPop->sample;
	list.head = nodeToPop->next;
	free(nodeToPop);
	list.nodes--;
	return LISTERROR_OK;
}

/* ListGetLengh retrieves amount of nodes stored in list.
** Arguments:
**  length:	nodes in list. Output.
** Return value:
**  LISTERROR_OK.
*/
listerrors_t	ListGetLength(uint8_t *const length)
{
	assert(length != 0);
	*length = list.nodes;
	return LISTERROR_OK;
}
