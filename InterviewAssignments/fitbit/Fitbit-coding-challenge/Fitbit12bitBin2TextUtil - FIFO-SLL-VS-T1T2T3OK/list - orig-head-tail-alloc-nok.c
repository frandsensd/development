#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "commonDefinitions.h"
#include "list.h"

static uint8_t	isInitialized = FALSE;
static singlelist_t	list;

static listerrors_t listInit()
{
	list.head = (singlelistnode_t*)malloc(sizeof(singlelistnode_t));
	list.tail = (singlelistnode_t*)malloc(sizeof(singlelistnode_t));
	if (list.head != NULL && list.tail != NULL)
	{
		list.head->next = list.tail;
		list.tail->next = list.tail;
		/* Ensure tail contains the largest value (unsigned integers) so it will remain tail */
		list.tail->sample = 0xffff;
		list.elements = 0;
		isInitialized = TRUE;
		return LISTERROR_OK;
	}
	else
	{
		return LISTERROR_MEMORYALLOCATIONFAILED;
	}
}

/* ListAddNode allocates newNode and inserts newNode after node.
** List management is updated and sample is added.
*/
static listerrors_t ListAddNode(const sample_t sample, singlelistnode_t *const node)
{
	singlelistnode_t* newNode = (singlelistnode_t*)malloc(sizeof(singlelistnode_t));
	if (newNode != NULL) {
		newNode->next = node->next;
		node->next = newNode;
		newNode->sample = sample;
		list.elements += (list.elements < SAMPLESTOSTORE) ? 1 : 0;
	}
	else {
		return LISTERROR_MEMORYALLOCATIONFAILED;
	}
	return LISTERROR_OK;
}


/* ListSearchForInsertionPoint compares value to sample in list starting from first sample
** pointed to by head->next.
** Order of insertion is ascending, i.e. smallest first.
** Hence ...
**
*/
static singlelistnode_t* ListSearchForInsertionPoint(const sample_t value)
{
	/* currNode -> first/smallest element in list.
	** prevNode -> previous node */
	singlelistnode_t* currNode = list.head->next;
	singlelistnode_t* prevNode = list.head;
	/* As long as sample is large enough, skip to next node unless we're at end of list */
	while (currNode->sample < sample && currNode->next != list.tail) {
		prevNode = currNode;
		currNode = currNode->next;
		printf("%s: advancing node; node->sample=%d < sample(%d)\n", __FUNCTION__, currNode->sample, sample);
	}
	/* currNode -> first element <= sample OR tail.
	** Thus insertion point is at currNode unless buffer is full and tail is reached
	** then sample is irrelevant */
	if (currNode->next == list.tail && SAMPLESTOSTORE == list.elements) {
		return NULL;
	}
	else {
		return prevNode;
	}
}

/* listInsert inserts sample in single linked list in ordered manner
** with head always pointing to largest value in list.
*/
listerrors_t	listInsert(const sample_t sample)
{
	if (FALSE == isInitialized) {
		listInit();
	}

	/* look for insertion point. 
	** Special case of first element first */
	if (0 == list.elements)
	{
		if (LISTERROR_MEMORYALLOCATIONFAILED == ListAddNode(sample, list.head)) {
			return LISTERROR_MEMORYALLOCATIONFAILED;
		}
		printf("%s:Added first element to list\n", __FUNCTION__);
		return LISTERROR_OK;
	}

	/* At least one element now exists in list. */
	singlelistnode_t* node = ListSearchForInsertionPoint(sample);
	if (NULL != node) {
		ListAddNode(sample, node);
		printf("%s: node inserted between samples %d and %d\n", __FUNCTION__, node->sample, node->next->next->sample);
	}
	else {
		printf("%s: sample (%d) too small to be inserted.\n", __FUNCTION__, sample);
	}
	return LISTERROR_OK;
}

/* listPop retrieves largest value from list (element head points at),
** updates head to point to next element and deletes the element.
*/
listerrors_t	listPop(sample_t *const sample)
{
	if (FALSE == isInitialized) {
		listInit();
	}
	singlelistnode_t* nodeToPop = list.head->next;
	*sample = nodeToPop->sample;
	list.head->next = nodeToPop->next;
	free(nodeToPop);
	return LISTERROR_OK;
}

/* listGetLengh retrieves amount of nodes stored in list.
*/
listerrors_t	listGetLength(uint8_t *const length)
{
	*length = list.elements;
	return LISTERROR_OK;
}
