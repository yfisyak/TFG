/* Copyright 1996, Lawrence Berkeley Laboratory */

/* dsmem.c - dynamic memory routines */
   
/*
modification history
--------------------
06jun96,whg  moved from othe files.
11jun96,whg  added indirection to dataset structure
*/

/*
DESCRIPTION
general routines for memory allocation
*/
#include <stdlib.h>
#include <string.h>
#define DS_PRIVATE
#include "dstype.h"
/*
 * fix for realloc bug on some versions of UNIX
 */
#define DS_REALLOC(ptr, size) ((ptr) ? realloc(ptr, size) : malloc(size))
/*
 * locations for memory allocation stats
 */
static int dsDsetSize = 0;
static int dsListSize = 0;
static int dsMemCalls = 0;
static int dsTidSize = 0;
static int dsTmpSize = 0;
/******************************************************************************
*
* dsAllocStats - print dataset allocation stats
*
* RETURNS: N/A
*/
void dsAllocStats(void)
{
	printf("AllocStats: dsetSize %d, listSize %d, tmpSize %d, memCalls %d, tidSize %d\n",
		dsDsetSize, dsListSize, dsTmpSize, dsMemCalls, dsTidSize);
}
/******************************************************************************
*
* dsFreeDataset - free memory for dataset
*
* RETURNS: TRUE if success else FALSE
*/
int dsFreeDataset(DS_DATASET_T *dataset)
{
	DS_LIST_T list;

	return 	dsMakeFreeList(&list, dataset) && dsFreeListed(&list);
}
/******************************************************************************
*
* dsFreeListed - free memory for datasets on list
*
* RETURNS: TRUE if success else FALSE
*/
int dsFreeListed(DS_LIST_T *list)
{
	size_t i;
	DS_DATASET_T *p;

	for (i = 0; i < list->count; i++) {
		p = list->pItem[i];
		if (p->p.data && (p->flags & DS_F_ALLOC_P) != 0) {
			p->elcount = 0;
			if (!dsRealloc(p, 0)) {
				dsListFree(list);
				return FALSE;
			}
		}
		if (p->flags & DS_F_ALLOC_NODE) {
			p->flags = DS_F_INVALID;
			free(p);
			dsMemCalls++;
			dsDsetSize -= sizeof(DS_DATASET_T);
		}
	}
	return dsListFree(list);
}
/*****************************************************************************
*
* dsListAppend - append item to list
*
* RETURN TRUE if success else FALSE
*/
int dsListAppend(DS_LIST_T *list, DS_DATASET_T *item)
{
	size_t size;
	void *ptr;

	if (!DS_IS_VALID(item)) {
		DS_ERROR(DS_E_INVALID_DATASET_OR_TABLE);
	}
	if (list->count >= list->maxcount) {
		if (list->count != list->maxcount) {
			DS_ERROR(DS_E_SYSTEM_ERROR);
		}
		size = (list->maxcount + DS_LIST_INC)*sizeof(DS_DATASET_T *);
		if ((ptr = DS_REALLOC(list->pItem, size)) == NULL) {
			DS_ERROR(DS_E_NOT_ENOUGH_MEMORY);
		}
		dsMemCalls++;
		dsListSize += DS_LIST_INC*sizeof(DS_DATASET_T *);
		list->pItem = ptr;
		list->maxcount += DS_LIST_INC;
	}
	list->pItem[list->count++] = item;
	return TRUE;
}
/*****************************************************************************
*
* dsListInit - initialize list structure
*
* RETURN TRUE if success else FALSE
*/
void dsListInit(DS_LIST_T *list)
{
	memset(list, 0 , sizeof(DS_LIST_T));
}
/*****************************************************************************
*
* dsListFree - free memory for list structure
*
* RETURN TRUE if success else FALSE
*/
int dsListFree(DS_LIST_T *list)
{
	if (list->pItem != NULL) {
		free(list->pItem);
		dsMemCalls++;
		dsListSize -= sizeof(DS_LIST_T *)*list->maxcount;
		memset(list, 0, sizeof(DS_LIST_T));
	}
	return TRUE;
}
/******************************************************************************
*
* dsMakeFreeList - make list of datasets to be freed and adjust refcount
*
* RETURNS: TRUE if success else FALSE
*/
int dsMakeFreeList(DS_LIST_T *list, DS_DATASET_T *dataset)
{
	size_t i, j;
	DS_DATASET_T *p;

	if (dataset == NULL) {
		DS_ERROR(DS_E_NULL_POINTER_ERROR);
	}
	dsListInit(list);
	/* form list of reachable nodes */
	if (!dsVisitList(list, dataset)) {
		goto fail;
	}
	/* clear visit for reachable nodes */
	for (i = 0; i < list->count; i++) {
		list->pItem[i]->visit = 0;
	}
	/* depth first traversal to count number of times node is visited */
	if (!dsVisitCount(dataset)) {
		goto fail;
	}
	if ((dataset->refcount + 1) != dataset->visit) {
		DS_LOG_ERROR(DS_E_NOTHING_TO_FREE);
		goto fail;
	}
	/* clear visit during depth first traversal of dataset */
	/* for nodes that have refcount > vist */
	for (i = 1; i < list->count; i++) {
		p = list->pItem[i];
		if (p->visit && p->refcount > p->visit && !dsVisitClear(p)) {
			goto fail;
		}
		else if (p->visit != 0 && p->visit != p->refcount) {
			DS_LOG_ERROR(DS_E_SYSTEM_ERROR);
			goto fail;
		}
	}
	/* adjust refcounts */
	for (i = 0; i < list->count; i++) {
		p = list->pItem[i];
		if (DS_IS_DATASET(p) && p->visit != 0) {
			for (j = 0; j < p->elcount; j++) {
				p->p.link[j]->refcount--;
			}
			p->elcount = 0;
		}
	}
	/* form list of nodes to be freed */
	for (i = j = 0; i < list->count; i++) {
		p = list->pItem[i];
		if (p->visit != 0) {
			if (p->refcount != 0) {
				DS_ERROR(DS_E_SYSTEM_ERROR);
			}
			list->pItem[j++] = p;
		}
	}
	list->count = j;
	return TRUE;
fail:
	dsListFree(list);
	return FALSE;
}
/*****************************************************************************
*
* dsNewDataset - create an empty dataset
* 
* RETURNS: TRUE if success else FALSE
*/
int dsNewDataset(DS_DATASET_T **ppDataset, char *name)
{
	char *tst;
	DS_DATASET_T *pDataset;

	if (ppDataset == NULL) {
		DS_ERROR(DS_E_NULL_POINTER_ERROR);
	}
	if ((pDataset = calloc(1, sizeof(DS_DATASET_T))) == NULL) {
		DS_ERROR(DS_E_NOT_ENOUGH_MEMORY);
	}
	dsMemCalls++;
	if (!dsParseName(pDataset->name, name, &tst) || *tst != '\0') {
		free(pDataset);
		dsMemCalls++;
		DS_ERROR(DS_E_INVALID_DATASET_NAME);	
	}
	dsDsetSize += sizeof(DS_DATASET_T);
	pDataset->flags |= DS_F_ALLOC_NODE;
	*ppDataset = pDataset;
	return TRUE;
}
/*****************************************************************************
*
* dsNewTable - create a table descriptor
*
* RETURNS: TRUE if success else FALSE
*/
int dsNewTable(DS_DATASET_T **ppTable, char *tableName,
	char *typeSpecifier, unsigned rowCount, void *pData)
{
	DS_DATASET_T *pTable;

	if (ppTable == NULL) {
		DS_ERROR(DS_E_NULL_POINTER_ERROR);
	}
	if ((pTable = malloc(sizeof(DS_DATASET_T))) == NULL) {
		return FALSE;
	}
	dsMemCalls++;
	if (!dsInitTable(pTable, tableName, typeSpecifier, rowCount, pData)) {
		free(pTable);
		dsMemCalls++;
		return FALSE;
	}
	pTable->flags |= DS_F_ALLOC_NODE;
	*ppTable = pTable;
	dsDsetSize += sizeof(DS_DATASET_T);
	return TRUE;
}
/*****************************************************************************
*
* dsRealloc - realloc dataset or table
*
* RETURN TRUE if success else FALSE
*/
int dsRealloc(DS_DATASET_T *dataset, size_t maxcount)
{
	void *ptr;
	size_t delta, elsize, size;
	DS_TYPE_T *type;

	if (!DS_IS_VALID(dataset) || !DS_IS_DYNAMIC(dataset) ||
		maxcount < dataset->elcount) {
		DS_ERROR(DS_E_REALLOC_ERROR);
	}
	if (DS_IS_TABLE(dataset)) {
		if (!dsTypePtr(&type, dataset->tid)) {
			return FALSE;
		}
		elsize = type->size;
	}
	else if (DS_IS_DATASET(dataset)) {
		elsize = sizeof(DS_DATASET_T *);
	}
	else {
		DS_ERROR(DS_E_SYSTEM_ERROR);
	}
	size = elsize*maxcount;
	delta = size - (dataset->p.data == NULL ? 0 : elsize*dataset->maxcount);
	if (size != 0) {
		if ((ptr = DS_REALLOC(dataset->p.data, size)) == NULL) {
			DS_ERROR(DS_E_NOT_ENOUGH_MEMORY);
		}
		dsMemCalls++;
		dataset->p.data = ptr;
		dataset->flags |= DS_F_ALLOC_P;
	}
	else if (dataset->p.data != NULL) {
		free(dataset->p.data);
		dsMemCalls++;
		dataset->p.data = NULL;
		dataset->flags &= ~DS_F_ALLOC_P;
	}
	dsDsetSize += delta;
	dataset->maxcount = maxcount;
	return TRUE;
}
/*****************************************************************************
*
* dsReallocTable - allocate memory for table
*
* RETURNS: TRUE if success else FALSE
*/
int dsReallocTable(DS_DATASET_T *pTable, size_t nRow)
{
	if (!DS_IS_TABLE(pTable)) {
		DS_ERROR(DS_E_INVALID_TABLE);
	}
	return dsRealloc(pTable, nRow);
}
/******************************************************************************
*
* dsTmpAlloc - allocate memory for tmp structures
*
* RETURNS: pointer to memory or NULL if size is zero or not enough memory
*/
void *dsTmpAlloc(size_t size)
{
	char *ptr;

	if (size == 0){
		DS_LOG_ERROR(DS_E_ZERO_LENGTH_ALLOC);
	}
	else if ((ptr = malloc(size)) != NULL) {
		dsMemCalls++;
		dsTmpSize += size;
		return ptr;
	}	
	else {
		DS_LOG_ERROR(DS_E_NOT_ENOUGH_MEMORY);
	}
	return NULL;
}
/******************************************************************************
*
* dsTmpFree - allocate memory for tmp structures
*
* RETURNS: pointer to memory or NULL if size is zero or not enough memory
*/
void dsTmpFree(void *ptr, size_t size)
{
	if (ptr != NULL) {
		free((char *)ptr);
		dsMemCalls++;
		dsTmpSize -= size;
	}
}
/******************************************************************************
*
* dsTypeCalloc - allocate zeroed memory for type structure
*
* RETURNS: pointer to memory or NULL if size is zero or not enough memory
*/
void *dsTypeCalloc(size_t size)
{
	char *ptr;

	if (size == 0){
		DS_LOG_ERROR(DS_E_ZERO_LENGTH_ALLOC);
	}
	else if ((ptr = calloc(1, size)) != NULL) {
		dsMemCalls++;
		dsTidSize += size;
		return ptr;
	}	
	else {
		DS_LOG_ERROR(DS_E_NOT_ENOUGH_MEMORY);
	}
	return NULL;
}
/******************************************************************************
*
* dsTypeFree - free memory for type structure
*
* RETURNS: N/A
*/
void dsTypeFree(void *ptr, size_t size)
{
	if (ptr != NULL) {
		free((char *)ptr);
		dsMemCalls++;
		dsTidSize -= size;
	}
}
