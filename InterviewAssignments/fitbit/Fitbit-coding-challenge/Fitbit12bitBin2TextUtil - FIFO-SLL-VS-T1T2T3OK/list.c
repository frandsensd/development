#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "commonDefinitions.h"
#include "list.h"

static uint8_t	isInitialized = FALSE;
static singlelist_t	list;

static listerrors_t listInit()
{
	singlelistnode_t*	node;
	node = (singlelistnode_t*)malloc(sizeof(singlelistnode_t));
	if (NULL == node) {
		return LISTERROR_MEMORYALLOCATIONFAILED;
	}
	list.head = node;
	list.tail = node;
	list.nodes = 0;
	isInitialized = TRUE;
	return LISTERROR_OK;
}

/* listInsert inserts sample in single linked list in ordered manner
** with head always pointing to smallest value node in list and tail 
** always pointing to largest value node in list.
*/
listerrors_t	listInsert(const sample_t newSample)
{
	singlelistnode_t*	node;
	if (FALSE == isInitialized) {
		/* NOTE: Creates new node and initializes head/tail to this node. */
		listInit();
		node = list.head;
		node->next = NULL;
		node->sample = newSample;
		list.nodes++;
		printf("%s:Added first sample(%d) to list\n", __FUNCTION__, newSample);
		return LISTERROR_OK;
	}
	else {
		/* Check if we need to create a new node for insertion: */
		if (list.nodes < SAMPLESTOSTORE || newSample >= list.tail->sample) {
			node = (singlelistnode_t*)malloc(sizeof(singlelistnode_t));
			if (NULL == node) {
				return LISTERROR_MEMORYALLOCATIONFAILED;
			}
			node->sample = newSample;
		}
		else {
			printf("%s:No new node created for sample %d\n", __FUNCTION__, newSample);
			return LISTERROR_OK;
		}
	}

	/* Node created, now insert */
	if (newSample <= list.head->sample) {
		/* insert at head node */
		node->next = list.head;
		list.head = node;
		list.nodes++;
		printf("%s: sample %d inserted as first node in list - nodes = %d.\n", __FUNCTION__, newSample, list.nodes);
	}
	else if (newSample >= list.tail->sample) {
		singlelistnode_t* t = list.head;
		/* insert at tail node */
		while (t->next != NULL) {
			t = t->next;
		}
		node->next = NULL;
		t->next = node;
		list.tail = node;
		if (SAMPLESTOSTORE == list.nodes) {
			/* list is full - we need to keep largest values, hence we discard the smallest one */
			singlelistnode_t* t = list.head;
			list.head = list.head->next;
			free(t);
		}
		else {
			list.nodes++;
		}
		printf("%s: sample %d inserted as last node in list - nodes = %d.\n", __FUNCTION__, newSample, list.nodes);
	}
	else {
		/* newSample is between min/max in list - locate insertion point and insert */
		singlelistnode_t* t = list.head;
		/* we already checked for boundaries above - lets look ahead another sample and get a win for insertion */
		while (newSample > t->next->sample) {
			t = t->next;
		}
		node->next = t->next;
		t = node;
		list.nodes++;
		printf("%s: sample %d inserted in list - nodes = %d.\n", __FUNCTION__, newSample, list.nodes);
	}
}

/* listPop retrieves smallest value from list (node head points at),
** updates head to point to next node and deletes the obsoleted node.
*/
listerrors_t	listPop(sample_t *const sample)
{
	if (FALSE == isInitialized) {
		printf("%s: Error - list not initialized!", __FUNCTION__);
		return LISTERROR_LISTNOTINITIALIZED;
	}
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

/* listGetLengh retrieves amount of nodes stored in list.
*/
listerrors_t	listGetLength(uint8_t *const length)
{
	*length = list.nodes;
	return LISTERROR_OK;
}
